#define _CRT_SECURE_NO_WARNINGS // C4996 에러 해결을 위해 최상단에 반드시 추가
#pragma once
#define MAX_VTXS 20
#define INF 999
#include <stdio.h>

class AdjMatGraph {
protected:
	int size;
	char vertices[MAX_VTXS];
	int adj[MAX_VTXS][MAX_VTXS];
public:
	AdjMatGraph()
	{
		reset();
	}
	char getVertex(int i) { return vertices[i]; }
	int getEdge(int i, int j) { return adj[i][j]; }
	void setEdge(int i, int j, int val) { adj[i][j] = val; }
	bool isEmpty() { return size == 0; }
	bool isFull() { return size >= MAX_VTXS; }

	///그래프 초기화==> 공백상태의 그래프 (오타 수정됨)
	void reset() {
		size = 0;
		for (int i = 0; i < MAX_VTXS; i++)
			for (int j = 0; j < MAX_VTXS; j++)
				setEdge(i, j, 0);
	}
	//정점 삽입
	void insertVertex(char name) {
		if (!isFull())vertices[size++] = name;
		else printf("Error: 그래프 정점 개수 초과\n");

	}
	//간선삽입: 무방향 그래프의 경우임 방향 가중치 그래프에서는수정
	void insertEdge(int u, int v) {
		setEdge(u, v, 1);
		setEdge(v, u, 1);
	}

	//그래프정보출력
	void display(FILE* fp = stdout) {
		fprintf(fp, "%d\n", size);
		for (int i = 0; i < size; i++) {
			fprintf(fp, "%c", getVertex(i));
			for (int j = 0; j < size; j++)
				fprintf(fp, "%5d", getEdge(i, j));
			fprintf(fp, "\n");

		}
	}
};

class SrchAMGraph : public AdjMatGraph {
	bool visited[MAX_VTXS] = { false }; // C26495 경고 해결 (기본값 초기화)
public:
	void resetVisited() {
		for (int i = 0; i < size; i++)
			visited[i] = false;

	}
	bool isLinked(int u, int v) {
		return getEdge(u, v) != 0;
	}

	//깊이 우선탐색함수
	void DFS(int v) {
		visited[v] = true;
		printf("%c", getVertex(v));
		for (int w = 0; w < size; w++)
			if (isLinked(v, w) && visited[w] == false)
				DFS(w);
	}
};

// ==========================================================
// 여기서부터 첨부하신 교재 이미지를 바탕으로 추가된 코드입니다.
// ==========================================================

// 가중치 그래프를 표현하는 클래스
class WGraph : public AdjMatGraph {
public:
	// 가중치를 포함한 간선 삽입 (오버로딩)
	void insertEdge(int u, int v, int weight) {
		if (weight > INF) weight = INF;
		setEdge(u, v, weight);
	}

	// 두 정점 간의 간선 존재 여부 확인 (가중치가 INF 미만이면 간선 존재)
	bool hasEdge(int i, int j) {
		return (getEdge(i, j) < INF);
	}

	// 텍스트 파일로부터 그래프 정보를 읽어오는 함수
	// (참고: 최신 C++ 컴파일러 환경의 문자열 리터럴 경고를 방지하기 위해 
	// 교재의 char* 대신 const char*로 사용하는 것이 좋으나, 교재 원문 그대로 반영했습니다.)
	void load(const char* filename) {
		FILE* fp = fopen(filename, "r");
		if (fp != NULL) {
			int n, val;
			fscanf(fp, "%d", &n); // 정점의 전체 개수

			for (int i = 0; i < n; i++) {
				char str[80];
				int val;
				fscanf(fp, "%s", str); // 정점의 이름
				insertVertex(str[0]);  // 정점 삽입

				for (int j = 0; j < n; j++) {
					fscanf(fp, "%d", &val); // 간선 정보(가중치)
					insertEdge(i, j, val);  // 간선 삽입
				}
			}
			fclose(fp);
		}
	}
};