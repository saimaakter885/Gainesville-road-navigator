#pragma once
#include <vector>

using std::vector;

// Forward Declaration for Node struct
struct Node;

struct HeapNode {
    Node* mapNode;
    double distance;
};

class MinHeap {
    vector<HeapNode> heap;

    void heapifyUp(int index);
    void heapifyDown(int index);

public:
    bool empty() const;
    void push(Node* node, double dist);
    Node* pop();
};