#include "kdTree.h"
#include "distance.h"
#include <algorithm>
#include <cmath>

KDNode* KDTree::buildTreeRecursive(std::vector<Node*>& nodes, int depth, int start, int end) {
    if (start > end) return nullptr;

    // Determine axis (0 for lat, 1 for lon)
    int axis = depth % 2;
    int mid = start + (end - start) / 2;

    // Find median and rearrange vector
    if (axis == 0) {
        std::nth_element(nodes.begin() + start, nodes.begin() + mid, nodes.begin() + end + 1,
            [](Node* a, Node* b) { return a->lat < b->lat; });
    } else {
        std::nth_element(nodes.begin() + start, nodes.begin() + mid, nodes.begin() + end + 1,
            [](Node* a, Node* b) { return a->lon < b->lon; });
    }

    // Create current node using median
    KDNode* tNode = new KDNode(nodes[mid]);
    
    tNode->left = buildTreeRecursive(nodes, depth + 1, start, mid - 1);
    tNode->right = buildTreeRecursive(nodes, depth + 1, mid + 1, end);

    return tNode;
}

void KDTree::freeTree(KDNode* node) {
    if (node == nullptr) return;

    // Postorder traversal for deletion
    freeTree(node->left);
    freeTree(node->right);
    delete node;
}

void KDTree::build(std::unordered_map<long long, Node>& allNodes) {
    // Create node vector
    std::vector<Node*> nodes;
    nodes.reserve(allNodes.size());

    for (const auto& pair : allNodes) {
        nodes.push_back(const_cast<Node*>(&pair.second));
    }

    root = buildTreeRecursive(nodes, 0, 0, nodes.size() - 1);
}

void KDTree::findNearestHelper(KDNode* currentNode, double targetLat, double targetLon, int depth, KDNode*& bestNode, double& bestDist) {
    if (currentNode == nullptr) return;

    // Get distance from target
    double currentDist = haversineDistance(currentNode->graphNode->lat, currentNode->graphNode->lon, targetLat, targetLon);

    // Check if current distance is better than best
    if (currentDist < bestDist) {
        bestDist = currentDist;
        bestNode = currentNode;
    }

    // Determine axis
    int axis = depth % 2;

    // Determine value to compare (0 for lat, 1 for lon)
    double targetVal;
    double currentVal;
    if (axis == 0) {
        targetVal = targetLat;
        currentVal = currentNode->graphNode->lat;
    } else {
        targetVal = targetLon;
        currentVal = currentNode->graphNode->lon;
    }

    // Choose left/right, save other side for comparison on unwind
    KDNode* nextBranch = nullptr;
    KDNode* otherBranch = nullptr;

    if (targetVal < currentVal) {
        nextBranch = currentNode->left;
        otherBranch = currentNode->right;
    } else {
        nextBranch = currentNode->right;
        otherBranch = currentNode->left;
    }

    findNearestHelper(nextBranch, targetLat, targetLon, depth + 1, bestNode, bestDist);

    // Calculate distance to split to determine need to check other branch
    double distToSplit;
    if (axis == 0) {
        distToSplit = haversineDistance(targetLat, targetLon, currentVal, targetLon); // Keep lon constant
    } else {
        distToSplit = haversineDistance(targetLat, targetLon, targetLat, currentVal); // Keep lat constant
    }

    // If distance to split is less than current best, check other branch
    if (distToSplit < bestDist) {
        findNearestHelper(otherBranch, targetLat, targetLon, depth + 1, bestNode, bestDist);
    }
}

Node* KDTree::findNearestNode(double lat, double lon) {
    if (root == nullptr) return nullptr;

    KDNode* bestNode = nullptr;
    double bestDist = std::numeric_limits<double>::max();

    findNearestHelper(root, lat, lon, 0, bestNode, bestDist);

    return bestNode->graphNode;
}