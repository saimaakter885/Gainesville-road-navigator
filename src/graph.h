#ifndef GAINESVILLE_ROAD_NAVIGATOR_GRAPH_H
#define GAINESVILLE_ROAD_NAVIGATOR_GRAPH_H

#include<vector>
#include<unordered_map>
using namespace std;


struct Node;

struct Edge
{
    Node* destination;
    double weight;
};

struct Node
{
    long long id;
    double lat;
    double lon;
    vector<Edge> edges;
};

class Graph
{
public:
    unordered_map<long long, Node> nodes;
    void addNode(long long id, double lat, double lon);
    void addEdge(long long StartId, long long endId, double weight);
    Node* getNode(long long id);
    void printGraphInfo();
};

#endif //GAINESVILLE_ROAD_NAVIGATOR_GRAPH_H