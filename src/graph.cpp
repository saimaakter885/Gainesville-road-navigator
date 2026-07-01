#include "graph.h"
#include <iostream>

void Graph::addNode(long long id, double lat, double lon)
{
    if (nodes.find(id) == nodes.end())
    {
        Node newNode;
        newNode.id = id;
        newNode.lat = lat;
        newNode.lon = lon;
        nodes[id] = newNode;
    }
}

Node* Graph::getNode(long long id)
{
    if (nodes.find(id) == nodes.end())
    {
        return nullptr;
    }
    return &nodes[id];
}

void Graph::addEdge(long long StartId, long long endId, double weight)
{
    if (nodes.find(StartId) == nodes.end() || nodes.find(endId) == nodes.end())
    {
        return;
    }
    Node* startNode = getNode(StartId);
    Node* endNode = getNode(endId);

    Edge forwardEdge;
    forwardEdge.destination = endNode;
    forwardEdge.weight = weight;
    startNode->edges.push_back(forwardEdge);

    Edge backwardEdge;
    backwardEdge.destination = startNode;
    backwardEdge.weight = weight;
    endNode->edges.push_back(backwardEdge);

}

void Graph::printGraphInfo()
{
    int totalEdge = 0;
    for (unordered_map<long long, Node>::iterator it = nodes.begin(); it != nodes.end(); it++)
    {
        totalEdge += it->second.edges.size();
    }
    cout << "Total Nodes: " << nodes.size() << endl;
    cout << "Total Edges: " << totalEdge / 2 << endl;
}


