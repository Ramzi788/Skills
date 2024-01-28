#include <iostream>
#include <vector>
#include <list>
#include <stdexcept>
#include <map>
using namespace std;

class Graph{
private: map<int,vector<int>> adjlist;
public:

    /*
     Requires: Two Vectors, start and end, both of type int.
     Effect: Creates a graph with start and end being the ordered list of edges.
     */
    Graph(const vector <int> &starts, const vector<int> &ends){
        if (starts.size() != ends.size()){
            throw invalid_argument("Equal vectors.");
        }
        for (int i = 0; i<starts.size(); i++){
            adjlist[starts[i]].push_back(ends[i]);
        }
    }
    /*
     Requires: nodeID
     Effect: Returns # of edges outgoing from NodeID.
     */
    int numOutgoing(const int nodeID) const{
        if(adjlist.find(nodeID) == adjlist.end()){
            throw invalid_argument("No node exists");
        }
        return adjlist.at(nodeID).size();
    }

    /*
     Requires: nodeID
     Effect: Returns the nodes that receive edges from nodeID.
     */
    const vector<int> &adjacent(const int nodeID) const
    {
        if (adjlist.find(nodeID) == adjlist.end())
        {
            throw invalid_argument("No node exists");
        }
        return adjlist.at(nodeID);
    }

    /*
    Requires: graph
    Effect: Returns a graph cycle if the function is able to find it.

    Test Cases:
     1. An empty graph is acyclic so the function does nothing.
     2. A graph with no leafs is a cyclic graph.
     3. Only integers.
     4. To remove a leaf, remove all connections to the leaf and remove the leaf itself to formulate a new graph.
     */
    vector<int> hasCycle(int nodeID, vector<bool> &visited, vector<int> &path)
    {
        visited[nodeID] = true;
        vector<int> List;
        path.push_back(nodeID);
        for (int i = 0; i < adjlist[nodeID].size(); i++)
        {
            if (!visited[adjlist[nodeID][i]])
            {
                List = hasCycle(adjlist[nodeID][i], visited, path);
            }
            else
            {
                for (int j = 0; j < path.size(); j++)
                {
                    if (path[j] == adjlist[nodeID][i])
                    {
                        for (int k = j; k < path.size(); k++)
                        {
                            List.push_back(path[k]);
                        }
                        List.push_back(adjlist[nodeID][i]);
                        return List;
                    }
                }
            }
        }
        path.pop_back();
        return List;
    }
};
