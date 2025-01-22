#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>
#include <queue>
#include <stack>
#include <limits>
#include <map>
#include <set>

using namespace std;

//structure for storing edges and distances
struct Person{
    double x;
    double y;

    Person(double xCord, double yCord) : x(xCord), y(yCord) {}
};

//Adjacency list for storing sparse graph
map<pair<double, double>, vector<pair<double, double> > > adjacencyList;

vector<Person> readDataset(string filePath){
    ifstream file(filePath);
    string line;
    vector<Person> persons;

    while(getline(file,line)){
        stringstream ss(line);
        double x,y;
        char comma;

        if (!(ss >> x >> comma >> y)) {
            cerr << "Error: Invalid line format in dataset." << endl;
            continue;
        }

        ss >> x >> comma >> y;
        //persons.push_back({x,y});
        persons.emplace_back(x,y);
    }

    return persons;
}

double calculateDistance(Person a, Person b){
    return sqrt(pow(a.x-b.x, 2) + pow(a.y - b.y, 2));
}

void completeGraph(vector<Person> &persons){
    cout<<"Complete Graph:\n";
    for(size_t i=0 ; i<persons.size() ; i++){
        for(size_t j=0 ; j<persons.size() ; j++){
            if(i != j){
                double distance = calculateDistance(persons[i], persons[j]);
                cout<<"Edge: (" << persons[i].x << ", " << persons[i].y << ") -> (" << persons[j].x << ", " << persons[j].y << ") Distance: " << distance << endl;
            }
        }
    }
}

void sparseGraph(vector<Person> &persons){
    cout<< "\nSparse Graph:\n";
    for(size_t i=0 ; i<persons.size() ; i++){
        vector<pair<double, pair<double, double> > > distances;

        for(size_t j=0 ; j<persons.size() ; j++){
            if(i != j){
                double distance = calculateDistance(persons[i], persons[j]);
                distances.push_back(make_pair(distance, make_pair(persons[j].x, persons[j].y)));
            }
        }

        sort(distances.begin(), distances.end());

        vector<pair<double, double> > neighbors;
        for(size_t k=0 ; k<3 ; k++){
            neighbors.push_back(distances[k].second);
            cout << "Edge: (" << persons[i].x << ", " << persons[i].y << ") -> (" << distances[k].second.first << ", " << distances[k].second.second << ")\n";
        }

        adjacencyList[make_pair(persons[i].x, persons[i].y)] = neighbors;
    }
}


void printNeighbors() {
    cout << "\nNeighbors in Sparse Graph:\n";
    for(auto node : adjacencyList){
        cout << "Person (" << node.first.first << ", " << node.first.second << ") -> ";
        for(auto neighbor : node.second){
            cout << "(" << neighbor.first << ", " << neighbor.second << ") ";
        }
        cout << endl;
    }
}

void dfs(pair<double, double> source, set<pair<double, double> > &visited){
    visited.insert(source);
    cout << "(" << source.first << ", " << source.second << ")  ";

    for(auto neighbor : adjacencyList[source]){
        if(visited.find(neighbor) == visited.end()) {
            dfs(neighbor, visited);
        }
    }
}

int main(){
    string filePath = "/Users/aditidhenge/Desktop/Graph Theory/Assignment1/Assignment1_dataset.csv";

    //read the dataset
    vector<Person> persons = readDataset(filePath);

    //complete connected graph
    completeGraph(persons);

    //sparse graph with three nearest neighbours
    sparseGraph(persons);

    printNeighbors();
    
    cout << "\nDepth First Search on Sparse Graph: \n";
    set<pair<double,double> > visited;
    for(auto person : persons){
        pair<double,double> source = make_pair(person.x,person.y);
        if(visited.find(source) == visited.end()) {
            cout << "Component :\n";
            dfs(source, visited);
            cout<< endl;
        }
    }
}