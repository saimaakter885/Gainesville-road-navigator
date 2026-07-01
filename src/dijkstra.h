//
// Created by saima on 6/28/2026.
//

#ifndef GAINESVILLE_ROAD_NAVIGATOR_DIJKSTRA_H
#define GAINESVILLE_ROAD_NAVIGATOR_DIJKSTRA_H

#include <unordered_map>
#include <vector>
#include <utility>
#include "graph.h"
#include "minHeap.h"

using std::vector;
using std::unordered_map;
using std::pair;

class Dijkstra {
    unordered_map<Node*, double> distances;
    unordered_map<Node*, Node*> previous;
    int nodesVisited = 0;
    double totalDistance = 0.0;
    long long executionTime = 0;

    void reset();
    vector<pair<double, double>> generateCoordVec(Node* start, Node* destination);

public:
    vector<pair<double, double>> findPath(Node* start, Node* destination);
    int getNodesVisited() const;
    double getTotalDistance() const;
    long long getExecutionTime() const;
};

#endif //GAINESVILLE_ROAD_NAVIGATOR_DIJKSTRA_H