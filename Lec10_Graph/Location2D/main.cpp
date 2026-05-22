#include <iostream>
#include <stack>
#include <deque>    
#include <cstdio>
#include "Location2D.h"

using namespace std;

const int MAZE_SIZE = 6;

char map[MAZE_SIZE][MAZE_SIZE] = {
    {'1', '1', '1', '1', '1', '1'},
    {'e', '0', '1', '0', '0', '1'},
    {'1', '0', '0', '0', '1', '1'},
    {'1', '0', '1', '0', '1', '1'},
    {'1', '0', '1', '0', '0', 'x'},
    {'1', '1', '1', '1', '1', '1'}
};

bool isValidLoc(int r, int c) {
    if (r < 0 || c < 0 || r >= MAZE_SIZE || c >= MAZE_SIZE) return false;
    else return map[r][c] == '0' || map[r][c] == 'x';
}

int main() {
    int ds_num;
    printf("DFS 데이터 구조 라이브러리 선택; 1)STACK, 2)DEQUE \n");
    scanf_s("%d", &ds_num);

    Location2D entry(1, 0);

    // [💡 핵심 해결책] 변수를 여기에 선언하면 switch문 안팎 어디서든 오류 없이 인식됩니다!
    bool dfs_success = false;

    switch (ds_num) {
    case 1: {
        stack<Location2D> locStack;
        locStack.push(entry);

        while (!locStack.empty()) {
            Location2D here = locStack.top();
            locStack.pop();

            int r = here.row, c = here.col;
            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("\n미로 탐색 성공\n");
                dfs_success = true; // 성공 기록
                break;
            }
            else {
                map[r][c] = '.';
                if (isValidLoc(r - 1, c)) locStack.push(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locStack.push(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locStack.push(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locStack.push(Location2D(r, c + 1));
            }
        }
        break;
    }

    case 2: {
        deque<Location2D> locDeque;
        locDeque.push_front(entry);

        while (!locDeque.empty()) {
            Location2D here = locDeque.front();
            locDeque.pop_front();

            int r = here.row, c = here.col;
            printf("(%d,%d) ", r, c);

            if (map[r][c] == 'x') {
                printf("\n미로 탐색 성공\n");
                dfs_success = true; // 성공 기록
                break;
            }
            else {
                map[r][c] = '.';
                if (isValidLoc(r - 1, c)) locDeque.push_front(Location2D(r - 1, c));
                if (isValidLoc(r + 1, c)) locDeque.push_front(Location2D(r + 1, c));
                if (isValidLoc(r, c - 1)) locDeque.push_front(Location2D(r, c - 1));
                if (isValidLoc(r, c + 1)) locDeque.push_front(Location2D(r, c + 1));
            }
        }
        break;
    }

    default:
        printf("잘못된 입력입니다.\n");
        break;
    }

    // [💡 추가] switch문이 다 끝나고 탈출한 직후, 실패했을 때만 딱 한 번 메시지를 띄웁니다.
    if (!dfs_success && (ds_num == 1 || ds_num == 2)) {
        printf("\n미로 탐색 실패\n");
    }

    // --- 지도 원상 복구 ---
    for (int i = 0; i < MAZE_SIZE; i++) {
        for (int j = 0; j < MAZE_SIZE; j++) {
            if (map[i][j] == '.') {
                map[i][j] = '0';
            }
        }
    }
    map[1][0] = 'e';

    printf("\n--- [BFS] ---\n");

    deque<Location2D> bfsDeque;
    bfsDeque.push_back(entry);

    bool bfs_success = false;

    while (!bfsDeque.empty()) {
        Location2D here = bfsDeque.front();
        bfsDeque.pop_front();

        int r = here.row, c = here.col;

        if (map[r][c] == '.') continue;

        printf("(%d,%d) ", r, c);

        if (map[r][c] == 'x') {
            printf("\n미로 탐색 성공\n");
            bfs_success = true;
            break;
        }
        else {
            map[r][c] = '.';

            if (isValidLoc(r - 1, c)) bfsDeque.push_back(Location2D(r - 1, c));
            if (isValidLoc(r + 1, c)) bfsDeque.push_back(Location2D(r + 1, c));
            if (isValidLoc(r, c - 1)) bfsDeque.push_back(Location2D(r, c - 1));
            if (isValidLoc(r, c + 1)) bfsDeque.push_back(Location2D(r, c + 1));
        }
    }

    if (!bfs_success) printf("\n미로 탐색 실패\n");

    return 0;
}
