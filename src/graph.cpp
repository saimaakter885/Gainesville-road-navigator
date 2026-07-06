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

void Graph::addPlace(string name, double lat, double lon)
{
    Place place;
    place.name = name;
    place.lat = lat;
    place.lon = lon;
    places.push_back(place);
}

Node* Graph::findNearestNode(double lat, double lon)
{
    Node* nearest = nullptr;
    double bestDistance = 100000000000.0;

    for (auto it = nodes.begin(); it != nodes.end(); it++)
    {
        Node* current = &(it->second);
        double dLat = current->lat - lat;
        double dLon = current->lon - lon;
        double distance = dLat * dLat + dLon * dLon;

        if (distance < bestDistance)
        {
            bestDistance = distance;
            nearest = current;
        }
    }
    return nearest;
}

void Graph::printSamplePlaces()
{
    cout << "Sample places:" << endl;
    int count = 0;
    for (size_t i = 0; i < places.size() && count < 20; i++)
    {
        cout << "- " << places[i].name << endl;
        count++;
    }
}


