#include "minHeap.h"
#include "graph.h"
#include <utility>
#include <stdexcept>

void MinHeap::heapifyUp(int index) {
    while (index > 0) {
        int parentIndex = (index - 1) / 2;
        if (heap[index].distance < heap[parentIndex].distance) {
            std::swap(heap[index], heap[parentIndex]);
            index = parentIndex;
        } else {
            break;
        }
    }
}

void MinHeap::heapifyDown(int index) {
    while (2 * index + 1 < static_cast<int>(heap.size())) {
        int leftChildIndex = 2 * index + 1;
        int rightChildIndex = 2 * index + 2;
        int smallestIndex = index;

        // Left child smaller than parent
        if (heap[leftChildIndex].distance < heap[smallestIndex].distance) {
            smallestIndex = leftChildIndex;
        }
        // Right child smaller than parent or left child
        if (rightChildIndex < static_cast<int>(heap.size()) && heap[rightChildIndex].distance < heap[smallestIndex].distance) {
            smallestIndex = rightChildIndex;
        }

        if (smallestIndex != index) {
            std::swap(heap[index], heap[smallestIndex]);
            index = smallestIndex;
        } else {
            break;
        }
    }
}

bool MinHeap::empty() const { return heap.empty(); }

void MinHeap::push(Node* node, double dist) {
    heap.push_back({node, dist});
    heapifyUp(static_cast<int>(heap.size()) - 1);
}

Node* MinHeap::pop() {
    if (heap.empty()) throw std::runtime_error("Heap is empty!");

    Node* minNode = heap[0].mapNode;
    heap[0] = heap.back();
    heap.pop_back();

    if (!heap.empty()) heapifyDown(0);

    return minNode;
}