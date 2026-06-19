#ifndef FLIGHTGRAPH_H
#define FLIGHTGRAPH_H

#include <string>
#include <vector>
#include <map>

/**
 * @brief 항공편(Edge, 간선) 정보를 담는 구조체
 */
struct Flight {
    std::string flightNumber;  // 항공편명 (예: KE1201)
    std::string airline;       // 항공사 이름 (예: 대한항공)
    std::string departure;     // 출발 공항 코드 (예: GMP)
    std::string destination;   // 도착 공항 코드 (예: CJU)
    int price;                 // 항공권 가격 (원 단위)
    std::string duration;      // 비행 소요 시간 (예: 1시간 10분)
};

/**
 * @brief 공항 정점(Vertex, 노드) 정보를 담는 구조체
 */
struct Airport {
    std::string code;          // 공항 3자리 코드 (예: ICN, GMP, CJU)
    std::string name;          // 공항 한글명 (예: 인천공항)
    int x = 0;                     // 원본 지도 이미지(819x1024) 상의 X 좌표
    int y = 0;                   // 원본 지도 이미지(819x1024) 상의 Y 좌표
};

/**
 * @brief 항공 네트워크를 관리하는 그래프 클래스 (인접 리스트 구조)
 */
class FlightGraph {
public:
    // 공항 코드를 Key로 사용하여 공항 상세 정보를 저장하는 Map (정점 집합)
    std::map<std::string, Airport> airports;

    // 출발 공항 코드를 Key로, 해당 공항에서 출발하는 항공편들의 목록을 Value로 가지는 인접 리스트
    std::map<std::string, std::vector<Flight>> adjList;

    /**
     * @brief 그래프에 공항(정점)을 추가하는 메서드
     */
    void addAirport(const std::string& code, const std::string& name, int x, int y) {
        airports[code] = Airport{ code, name, x, y };
    }

    /**
     * @brief 그래프에 항공편(단방향 간선)을 추가하는 메서드
     */
    void addFlight(const std::string& num, const std::string& airline,
        const std::string& from, const std::string& to, int price, const std::string& duration) {
        adjList[from].push_back(Flight{ num, airline, from, to, price, duration });
    }
};

#endif // FLIGHTGRAPH_H