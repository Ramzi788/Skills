#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <map>
#include "my_graph.cpp"

int main(){
    vector<int> starts = {0, 0, 4, 3};
    vector<int> ends = {1, 2, 2, 3};
    Graph graph(starts, ends);
    cout<<graph.numOutgoing(4)<<endl; // Assessing the success of numOutgoing Function


    vector<bool> visited(5, false);
    vector<int> path;
    vector<int> list = graph.hasCycle(0, visited, path);
    for (int i = 0; i < list.size(); i++)
    {
        cout << list[i] << " ";
    }
    cout << endl;


    vector<int> adjList = graph.adjacent(4);
    for (int i = 0; i < adjList.size(); i++)
    {
        cout << adjList[i] << " ";

    }
    cout << endl;

    return 0;

}