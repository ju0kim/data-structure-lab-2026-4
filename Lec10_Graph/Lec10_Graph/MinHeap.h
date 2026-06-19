#pragma once
#include <iostream>

#define MAX_ELEMENT 200

// 힙에 저장할 노드 클래스
class HeapNode {
    int key; // Key 값: 간선의 가중치
    int v1;  // 정점 1
    int v2;  // 정점 2
public:
    HeapNode(int k = 0, int u = 0, int v = 0) : key(k), v1(u), v2(v) {}
    void setKey(int k, int u, int v) { key = k; v1 = u; v2 = v; }
    int getKey() { return key; }
    int getV1() { return v1; }
    int getV2() { return v2; }
};

// 최소 힙 클래스 (교재 생략 부분 포함)
class MinHeap {
    HeapNode node[MAX_ELEMENT];
    int size;
public:
    MinHeap() : size(0) {}
    bool isEmpty() { return size == 0; }
    bool isFull() { return size == MAX_ELEMENT - 1; }

    // 삽입 함수
    void insert(int key, int u, int v) {
        if (isFull()) return;
        int i = ++size;
        // 부모 노드(i/2)와 키값을 비교하며 위로 올라감
        while (i != 1 && key < node[i / 2].getKey()) {
            node[i] = node[i / 2];
            i /= 2;
        }
        node[i].setKey(key, u, v);
    }

    // 삭제 함수 (가장 작은 가중치를 가진 루트 노드 반환)
    HeapNode remove() {
        if (isEmpty()) return HeapNode(-1, -1, -1);

        HeapNode root = node[1];        // 최솟값(루트)
        HeapNode lastNode = node[size--]; // 힙의 마지막 노드

        int parent = 1;
        int child = 2;

        while (child <= size) {
            // 더 작은 자식 노드 찾기
            if (child < size && node[child].getKey() > node[child + 1].getKey())
                child++;
            // 마지막 노드가 들어갈 자리인지 확인
            if (lastNode.getKey() <= node[child].getKey()) break;

            node[parent] = node[child];
            parent = child;
            child *= 2;
        }
        node[parent] = lastNode;
        return root;
    }
};