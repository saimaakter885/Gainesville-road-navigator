#pragma once
#include <vector>
#include "graph.h"

struct KDNode {
    Node* graphNode;
    KDNode* left;
    KDNode* right;

    KDNode(Node* node) : graphNode(node), left(nullptr), right(nullptr) {}
};

class KDTree {
    KDNode* root;

    KDNode* buildTreeRecursive(std::vector<Node*>& nodes, int depth, int start, int end);
    void freeTree(KDNode* node);
    void findNearestHelper(KDNode* currentNode, double targetLat, double targetLon, int depth, KDNode*& bestNode, double& bestDist);

public:
    void build(std::unordered_map<long long, Node>& allNodes);
    Node* findNearestNode(double lat, double lon);
    ~KDTree() { freeTree(root); }
};