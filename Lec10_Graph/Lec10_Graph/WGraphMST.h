#pragma once
#include "WGraph.h"
#include "VertexSets.h"
#include "MinHeap.h"

class WGraphMST : public WGraph {
public:
    // ==========================================
    // [1] Kruskal 알고리즘
    // ==========================================
    void Kruskal() {
        MinHeap heap;
        for (int i = 0; i < size - 1; i++) {
            for (int j = i + 1; j < size; j++) {
                if (hasEdge(i, j)) {
                    heap.insert(getEdge(i, j), i, j);
                }
            }
        }

        VertexSets set(size);
        int edgeAccepted = 0;

        while (edgeAccepted < size - 1) {
            HeapNode e = heap.remove();
            int uset = set.findSet(e.getV1());
            int vset = set.findSet(e.getV2());

            if (uset != vset) {
                printf("간선 추가 : %c - %c (비용:%d)\n",
                    getVertex(e.getV1()), getVertex(e.getV2()), e.getKey());
                set.unionSets(uset, vset);
                edgeAccepted++;
            }
        }
    }

    // ==========================================
    // [2] Prim 알고리즘 (새로 추가된 부분)
    // ==========================================
    // MST에 포함되지 않은 정점들 중에서 MST와의 거리가 최소인 정점 선택
    int getMinVertex(bool* selected, int* dist) {
        int minv = 0;
        int mindist = INF;
        for (int v = 0; v < size; v++) {
            if (!selected[v] && dist[v] < mindist) {
                mindist = dist[v];
                minv = v;
            }
        }
        return minv;
    }

    // Prim의 MST 구현
    void Prim(int s) {
        bool selected[MAX_VTXS]; // 정점이 이미 포함되었는가?
        int dist[MAX_VTXS];      // 거리

        for (int i = 0; i < size; i++) { // 배열 초기화
            dist[i] = INF;
            selected[i] = false;
        }

        dist[s] = 0; // 시작 정점

        for (int i = 0; i < size; i++) {
            int u = getMinVertex(selected, dist);
            selected[u] = true;

            if (dist[u] == INF) return;

            printf("%c ", getVertex(u));

            for (int v = 0; v < size; v++) {
                if (getEdge(u, v) != INF) {
                    if (!selected[v] && getEdge(u, v) < dist[v]) {
                        dist[v] = getEdge(u, v);
                    }
                }
            }
        }
        printf("\n");
    }
};