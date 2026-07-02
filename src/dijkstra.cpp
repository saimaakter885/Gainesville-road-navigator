//
// Created by saima on 6/28/2026.
//

#include "dijkstra.h"
#include <unordered_set>
#include <chrono>
#include "minHeap.h"

using std::unordered_set;

vector<pair<double, double>> Dijkstra::findPath(Node* start, Node* destination) {
    // Initialization
    MinHeap heap;
    unordered_set<Node*> visited;
    reset();
    distances[start] = 0.0;
    previous[start] = nullptr;
    heap.push(start, 0.0);

    // Start execution time clock
    auto startTime = std::chrono::high_resolution_clock::now();

    // Visit every node in graph until destination found
    while (!heap.empty()) {
        Node* currNode = heap.pop();
        double currDistance = distances[currNode];

        // Check duplicate
        if (visited.find(currNode) != visited.end()) continue;

        // Check if destination reached
        if (currNode == destination) break;

        visited.insert(currNode);

        // Get distances to all neighbors
        for (Edge edge : currNode->edges) {
            Node* neighbor = edge.destination;
            double tentativeDistance = currDistance + edge.weight;

            // Add node if doesn't exist or distance is shorter
            if (distances.find(neighbor) == distances.end()) {
                heap.push(neighbor, tentativeDistance);
                previous[neighbor] = currNode;
                distances[neighbor] = tentativeDistance;
            } else if (tentativeDistance < distances[neighbor]) {
                heap.push(neighbor, tentativeDistance);
                distances[neighbor] = tentativeDistance;
                previous[neighbor] = currNode;
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