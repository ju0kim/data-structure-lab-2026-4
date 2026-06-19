#pragma warning(disable: 4996) // Qt 내부 권장기능 경고(C4996)를 에러로 잡지 않도록 비활성화
#ifndef MAPWIDGET_H
#define MAPWIDGET_H

#include <QLabel>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QPoint>
#include "FlightGraph.h" // 데이터 의존성 포함

/**
 * @brief 지도 이미지를 배경으로 깔고, 공항 간 노선을 화면에 직접 그리는 커스텀 위젯
 */
class MapWidget : public QLabel {
private:
    FlightGraph& graph;             // 메인 윈도우에서 관리하는 그래프 데이터의 참조 변수
    std::string selectedAirport;    // 현재 사용자가 클릭하여 선택한 공항의 코드

    const int ORIGINAL_MAP_WIDTH = 819;
    const int ORIGINAL_MAP_HEIGHT = 1024;

    /**
     * @brief 원본 이미지 좌표를 현재 창 크기에 맞게 비율을 계산하여 변환해주는 헬퍼 메서드
     */
    QPoint scalePoint(int x, int y) const {
        double sx = (double)width() / ORIGINAL_MAP_WIDTH;
        double sy = (double)height() / ORIGINAL_MAP_HEIGHT;
        return QPoint(static_cast<int>(x * sx), static_cast<int>(y * sy));
    }

public:
    MapWidget(FlightGraph& g, QWidget* parent = nullptr) : QLabel(parent), graph(g) {
        setFixedSize(560, 700);       // UI 배치 안정을 위해 지도 위젯 크기 고정
        setAlignment(Qt::AlignCenter); // 텍스트 혹은 에러 메시지 출력 시 중앙 정렬

        QPixmap pixmap("map.png");    // 실행 파일과 같은 경로에 있는 map.png 로드
        if (pixmap.isNull()) {
            setText("<h3 style='color:gray;'>[map.png 지도를 찾을 수 없습니다]<br>"
                "좌측 탐색기에 map.png 파일을 정확히 업로드해주세요.</h3>");
            setStyleSheet("background-color: #222226; border: 1px solid #444;");
        }
        else {
            setPixmap(pixmap.scaled(560, 700, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }

    /**
     * @brief 사용자가 공항을 선택했을 때 호출되어 강조할 노선을 갱신하는 메서드
     */
    void setSelectedAirport(const std::string& code) {
        selectedAirport = code;
        update(); // paintEvent() 재호출 강제 트리거
    }

protected:
    /**
     * @brief QLabel의 그리기 기능을 확장하여 항공 노선(선)을 그리는 페인트 이벤트
     */
    void paintEvent(QPaintEvent* event) override {
        QLabel::paintEvent(event);
        Q_UNUSED(event);

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);

        for (const auto& pair : graph.adjList) {
            for (const auto& flight : pair.second) {
                auto fromIt = graph.airports.find(flight.departure);
                auto toIt = graph.airports.find(flight.destination);

                if (fromIt == graph.airports.end() || toIt == graph.airports.end())
                    continue;

                const Airport& fromVertex = fromIt->second;
                const Airport& toVertex = toIt->second;

                if (fromVertex.x <= 0 || fromVertex.y <= 0 || toVertex.x <= 0 || toVertex.y <= 0)
                    continue;

                if (flight.departure == selectedAirport || flight.destination == selectedAirport) {
                    painter.setPen(QPen(QColor(255, 215, 0), 3)); // 활성화 노선은 노란색
                }
                else {
                    painter.setPen(QPen(QColor(0, 86, 179, 60), 1, Qt::DotLine)); // 일반 노선은 파란 점선
                }

                QPoint p1 = scalePoint(fromVertex.x, fromVertex.y);
                QPoint p2 = scalePoint(toVertex.x, toVertex.y);
                painter.drawLine(p1, p2);
            }
        }
    }
};

#endif // MAPWIDGET_H