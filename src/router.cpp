#include "router.h"
#include <algorithm>

void Router::reset() {
    distances.clear();
    previous.clear();
    nodesVisited = 0;
    executionTime = 0;
    totalDistance = 0.0;
}

// Turns Node path into coordinate pair path for clean output
vector<pair<double, double>> Router::generateCoordVec(Node* start, Node* destination) {
    vector<pair<double, double>> path;

    // Return empty vector if destination not found
    if (previous.find(destination) == previous.end() && start != destination) return path;

    // Trace backward using previous map
    Node* curr = destination;
    while (curr != nullptr) {
        path.push_back({curr->lat, curr->lon});
        curr = previous[curr];
    }

    // Reverse path to produce coordinates from start -> destination
    std::reverse(path.begin(), path.end());

    return path;
}

int Router::getNodesVisited() const { return nodesVisited; }

double Router::getTotalDistance() const { return totalDistance; }

long long Router::getExecutionTime() const { return executionTime; }