#include "AdjMatGraph.h"

void main() {
	// ==========================================
	// 1. 기존 탐색 그래프(SrchAMGraph) 테스트
	// ==========================================
	SrchAMGraph g;

	for (int i = 0; i < 4; i++)
		g.insertVertex('A' + i);
	g.insertEdge(0, 1);
	g.insertEdge(0, 3);
	g.insertEdge(1, 2);
	g.insertEdge(1, 3);
	g.insertEdge(2, 3);

	printf("인접행렬로 표현한 그래프\n");
	g.display();

	// 파일 출력
	FILE* fp;
	fopen_s(&fp, "../text_out.txt", "w");
	if (fp != NULL) { // 경고(C6387) 해결을 위한 NULL 체크 추가
		g.display(fp);
		fclose(fp);
	}

	printf("DFS ==> ");
	g.resetVisited();
	g.DFS(0);
	printf("\n\n");


	// ==========================================
	// 2. 덧붙인 가중치 그래프(WGraph) 테스트
	// ==========================================
	printf("=========================================\n");
	printf(" 가중치 그래프(WGraph) 파일 로드 테스트\n");
	printf("=========================================\n");

	WGraph wg;

	// 프로젝트 폴더 안에 있는 graph.txt 파일을 읽어옵니다.
	// (만약 text_out.txt처럼 상위 폴더에 두셨다면 "../graph.txt"로 경로를 수정하세요!)
	wg.load("../wgraph.txt");

	printf("텍스트 파일(graph.txt)에서 읽어온 가중치 그래프:\n");
	wg.display();
}