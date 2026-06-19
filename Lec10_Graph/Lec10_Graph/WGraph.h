#pragma once
#pragma warning(disable:4996) // 비주얼 스튜디오의 C4996 에러를 강제로 무시합니다.

#include "AdjMatGraph.h"        
#include <cstdio>

#define INF 9999 // 값이 INF 이상이면 간선이 없음

class WGraph : public AdjMatGraph {
public:
    // 가중치가 있는 간선 삽입
    void insertEdge(int u, int v, int weight) {
        if (weight > INF) weight = INF;
        setEdge(u, v, weight);
    }

    bool hasEdge(int i, int j) {
        return (getEdge(i, j) < INF);
    }

    // C언어 스타일의 가중치 그래프 파일 읽기 (교재 내용 반영)
    void load(const char* filename) {
        FILE* fp = fopen(filename, "r");
        if (fp != NULL) {
            int n, val;
            fscanf(fp, "%d", &n);           // 정점의 전체 개수
            for (int i = 0; i < n; i++) {
                char str[80];
                fscanf(fp, "%s", str);      // 정점의 이름
                insertVertex(str[0]);       // 정점 삽입

                for (int j = 0; j < n; j++) {
                    fscanf(fp, "%d", &val); // 간선 정보
                    insertEdge(i, j, val);  // 간선 삽입
                }
            }
            fclose(fp);
        }
        else {
            printf("오류: %s 파일을 열 수 없습니다.\n", filename);
        }
    }
};