#pragma once
#include <unordered_map>
#include <vector>
#include <utility>
#include "graph.h"

using std::unordered_map;
using std::vector;
using std::pair;

class Router {
protected:
    unordered_map<Node*, double> distances;
    unordered_map<Node*, Node*> previous;

    int nodesVisited = 0;
    long long executionTime = 0;
    double totalDistance = 0.0;

    void reset();
    vector<pair<double, double>> generateCoordVec(Node* start, Node* destination);

public:
    virtual ~Router() = default;

    virtual vector<pair<double, double>> findPath(Node* start, Node* destination) = 0;
    
    long long getExecutionTime() const;
    int getNodesVisited() const;
    double getTotalDistance() const;
};