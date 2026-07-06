#ifndef GAINESVILLE_ROAD_NAVIGATOR_GRAPH_H
#define GAINESVILLE_ROAD_NAVIGATOR_GRAPH_H

#include<vector>
#include<unordered_map>
#include <iostream>
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

struct Place
{
    string name;
    double lat;
    double lon;
};

class Graph
{
public:
    unordered_map<long long, Node> nodes;
    vector<Place> places;
    void addNode(long long id, double lat, double lon);
    void addEdge(long long StartId, long long endId, double weight);
    Node* getNode(long long id);
    void printGraphInfo();

    void addPlace(string name, double lat, double lon);
    Node* findNearestNode(double lat, double lon);
    void printSamplePlaces();
};

#endif //GAINESVILLE_ROAD_NAVIGATOR_GRAPH_H