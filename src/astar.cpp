//
// Created by saima on 6/28/2026.
//

#include <unordered_set>
#include "astar.h"
#include "minHeap.h"
#include "distance.h"

vector<pair<double, double>> Astar::findPath(Node* start, Node* destination) {
    // Initialization
    MinHeap heap;
    std::unordered_set<Node*> visited;
    reset();
    distances[start] = 0.0;
    previous[start] = nullptr;
    heap.push(start, 0.0);

    // Start execution time clock
    auto startTime = std::chrono::high_resolution_clock::now();

    // Run until destination found
    while (!heap.empty()) {
        Node* currNode = heap.pop();
        double currDistance = distances[currNode];

        // Check duplicate
        if (visited.find(currNode) != visited.end()) continue;

        // Check destination
        if (currNode == destination) break;

        visited.insert(currNode);

        // Get distances to all neighbors
        for (Edge edge : currNode->edges) {
            Node* neighbor = edge.destination;
            double roadDistance = currDistance + edge.weight;
            double distanceToDest = haversineDistance(neighbor->lat, neighbor->lon, destination->lat, destination->lon);
            double heuristicDist = roadDistance + distanceToDest;

            // Add node if doesn't exist or distance is shorter
            if (distances.find(neighbor) == distances.end()) {
                distances[neighbor] = roadDistance;
                previous[neighbor] = currNode;
                heap.push(neighbor, heuristicDist);
            } else if (roadDistance < distances[neighbor]) {
                distances[neighbor] = roadDistance;
                previous[neighbor] = currNode;
                heap.push(neighbor, heuristicDist);
            }
        }
    }

    // Stop execution clock to purely measure pathfinding time
    auto endTime = std::chrono::high_resolution_clock::now();

    // Update telemetry metrics
    nodesVisited = visited.size() + 1;  // Add 1 for destination node
    totalDistance = distances[destination];
    executionTime = std::chrono::duration_cast<std::chrono::microseconds>(endTime - startTime).count();

    // Create and return coordinate pair vector
    return generateCoordVec(start, destination);
}