#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <thread>

#include <thread>
#include <unistd.h>
#include <fstream>
#include <sstream>
#include <climits>

using namespace std;

struct Node {
    int src;
    int dest;
    int dist;

    void print(){
        cout << src << ", " << dest << ", " << dist << endl;
    }
};

vector<int> costs;
vector<long> thread_ids;
bool STOP_FLAG = false;
int no_of_elements=0;
int tieCount = 0;

void readFromFile(const string &filename, vector<int> &col1, vector<int> &col2, vector<int> &col3){
    ifstream file(filename);
    string line;

    while (getline(file, line)){
        stringstream ss(line);
        string value;
        getline(ss, value, ',');
        col1.push_back(stoi(value));
        getline(ss, value, ',');
        col2.push_back(stoi(value));
        getline(ss, value, ',');
        col3.push_back(stoi(value));
    }
    file.close();
}

void findShortestPath(vector<Node>& nodes){
    int num_nodes = nodes.size();

    // Initialize variables to track visited nodes and final path
    vector<bool> visited(num_nodes, false);
    vector<int> path;

    // Choose a random starting node
    int start_node = rand() % num_nodes;
    int current_node = start_node;
    path.push_back(current_node);
    visited[current_node] = true;

    // Repeat until all nodes are visited
    while (path.size() < num_nodes) {
        int nearest_node = -1;
        int min_distance = INT_MAX;

        // Find the nearest unvisited node
        for (int i = 0; i < num_nodes; ++i) {
            if (!visited[i] && nodes[current_node].dest == i && nodes[current_node].dist < min_distance) {
                min_distance = nodes[current_node].dist;
                nearest_node = i;
            }
        }

        // If no nearest unvisited node found, break the loop
        if (nearest_node == -1) break;

        // Add the nearest node to the path
        path.push_back(nearest_node);
        visited[nearest_node] = true;
        current_node = nearest_node;
    }

    // Print the path
    int total_cost = 0;
    // cout<<"\nShortest path: ";
    for (int node : path) {
        // cout << node << " ";
        total_cost += nodes[node].dist;
    }
    // cout<<start_node<<endl;
    cout<<"Thread id: "<<pthread_self()<<"\nTotal cost: "<<total_cost<<"\n\n";

    costs.push_back(total_cost);
    thread_ids.push_back(pthread_self());
    --no_of_elements;
    
    while(no_of_elements!=0);

    int cost = INT_MAX;
    long sm_thread_id = 0;
    for(int i=0; i<thread_ids.size(); i++){
        if(costs[i] < cost){
            cost = costs[i];
            sm_thread_id = thread_ids[i];
        }
    }
    // cout<<cost<<" "<<sm_thread_id<<endl;
    if(sm_thread_id==pthread_self()){
        cout<<"Thread ID: "<<sm_thread_id<<" has the smallest cost: "<<cost<<endl;
        STOP_FLAG = true;
    }
    
    while(1){
        if(STOP_FLAG)
            break;
    }

    //if there are multiple threads with same smaller cost then tie breaker
    for(int i=0; i<thread_ids.size(); i++){
        if(cost==costs[i])
            tieCount++;
    }
}



int main() {
    srand(time(0));

    vector<int> src, dest, dist;
    readFromFile("testFile.csv", src, dest, dist);

    vector<Node> nodes;
    for (int i = 0; i < src.size(); i++) {
        Node node;
        node.src = src[i];
        node.dest = dest[i];
        node.dist = dist[i];

        nodes.push_back(node);
        // node.print();
    }
    int n;
    cout<<"Enter the number of threads: ";
    cin>>n;
    no_of_elements=n;
    pthread_t threads[n];
    for(int i=0; i<n; i++){
        pthread_create(&threads[i], NULL, (void* (*)(void*))findShortestPath, &nodes);
    }
    for(int i=0; i<n; i++)
        pthread_join(threads[i], NULL);

    if(tieCount>2)
        cout<<"\nTieBreacker";


    return 0;
}