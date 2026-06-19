#include <iostream>
#include "AdjMatGraph.h"
#include "SrchAMGraph.h"
#include "WGraph.h"
#include "WGraphMST.h"
#include "WGraphDijkstra.h"

using namespace std;

int main() {
    // 동적 할당(new)을 사용하여 메모리 부족(Stack Overflow/액세스 위반) 문제를 원천 차단합니다.
    // 객체를 포인터(*)로 생성했기 때문에 점(.) 대신 화살표(->)를 사용합니다.

    // ========================================================
    // [1] 기본 인접 행렬 방식 테스트
    // ========================================================
    printf("=== 1. 인접 행렬로 표현한 그래프 (기본) ===\n");
    AdjMatGraph* g1 = new AdjMatGraph();
    for (int i = 0; i < 4; i++) g1->insertVertex('A' + i);
    g1->insertEdge(0, 1); g1->insertEdge(0, 3);
    g1->insertEdge(1, 2); g1->insertEdge(1, 3); g1->insertEdge(2, 3);
    g1->display();
    printf("\n");

    // ========================================================
    // [2] DFS 구현 테스트
    // ========================================================
    printf("=== 2. DFS 탐색 테스트 (graph.txt) ===\n");
    SrchAMGraph* g2 = new SrchAMGraph();
    g2->load("graph.txt");
    printf("DFS ==> ");
    g2->resetVisited();
    g2->DFS(0);
    printf("\n\n");

    // ========================================================
    // [3] 인접 행렬 가중치 그래프 테스트
    // ========================================================
    printf("=== 3. 가중치 그래프 테스트 (weight_graph.txt) ===\n");
    WGraph* g3 = new WGraph();
    g3->load("wgraph.txt");
    printf("가중치 그래프 화면 출력:\n");
    g3->display();
    printf("\n");

    // ========================================================
    // [4] Kruskal 알고리즘 (MST)
    // ========================================================
    printf("=== 4. Kruskal 알고리즘 테스트 ===\n");
    WGraphMST* g4 = new WGraphMST();
    g4->load("wgraph.txt");
    printf("MST By Kruskal's Algorithm\n");
    g4->Kruskal();
    printf("\n");

    // ========================================================
    // [5] Prim 알고리즘 (MST)
    // ========================================================
    printf("=== 5. Prim 알고리즘 테스트 ===\n");
    printf("MST By Prim's Algorithm (시작 정점 A)\n");
    g4->Prim(0);
    printf("\n");

    // ========================================================
    // [6] Dijkstra 최단 경로 테스트
    // ========================================================
    printf("=== 6. Dijkstra 알고리즘 테스트 (graph_sp.txt) ===\n");
    WGraphDijkstra* g6 = new WGraphDijkstra();
    g6->load("graph_sp.txt");
    printf("Shortest Path By Dijkstra Algorithm\n");
    g6->ShortestPath(0);
    printf("\n");

    // 다 쓴 메모리는 깔끔하게 반납합니다.
    delete g1;
    delete g2;
    delete g3;
    delete g4;
    delete g6;

    return 0;
}