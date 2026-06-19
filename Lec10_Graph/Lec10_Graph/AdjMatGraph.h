#pragma once
#define _CRT_SECURE_NO_WARNINGS 

#include <cstdio>
#include <iostream>
#include <fstream>
#include <string>

#define MAX_VTXS 256

using namespace std;

// 인접 행렬을 이용한 그래프 구현
class AdjMatGraph {
protected:
    int    size;                    // 정점의 개수
    char   vertices[MAX_VTXS];      // 정점 정보
    int    adj[MAX_VTXS][MAX_VTXS]; // 인접 행렬

public:
    AdjMatGraph() { reset(); }

    char getVertex(int i) { return vertices[i]; }
    int  getEdge(int i, int j) { return adj[i][j]; }
    void setEdge(int i, int j, int val) { adj[i][j] = val; }
    bool isEmpty() { return size == 0; }
    bool isFull() { return size >= MAX_VTXS; }

    // 그래프 초기화 ==> 공백 상태의 그래프
    void reset() {
        size = 0;
        for (int i = 0; i < MAX_VTXS; i++)
            for (int j = 0; j < MAX_VTXS; j++)
                setEdge(i, j, 0);
    }

    // 정점 삽입
    void insertVertex(char name) {
        if (!isFull()) vertices[size++] = name;
        else printf("Error: 그래프 정점 개수 초과\n");
    }

    // 간선 삽입
    void insertEdge(int u, int v) {
        setEdge(u, v, 1);
        setEdge(v, u, 1);
    }

    // --- 이 함수가 빠져있어서 에러가 났습니다! ---
    void load(const string& filename) {
        ifstream ifs(filename);
        if (!ifs.is_open()) return;

        ifs >> size;
        for (int i = 0; i < size; i++) {
            ifs >> vertices[i];
            for (int j = 0; j < size; j++) {
                ifs >> adj[i][j];
            }
        }
        ifs.close();
    }

    // 화면 출력
    void display() {
        printf("%d\n", size);
        for (int i = 0; i < size; i++) {
            printf("%c ", getVertex(i));
            for (int j = 0; j < size; j++)
                printf(" %3d", getEdge(i, j));
            printf("\n");
        }
    }
};