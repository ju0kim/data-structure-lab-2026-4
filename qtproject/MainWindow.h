#pragma warning(disable: 4996) // Qt 내부 권장기능 경고(C4996)를 에러로 잡지 않도록 비활성화
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QLocale>
#include "FlightGraph.h"
#include "MapWidget.h"

/**
 * @brief 애플리케이션의 메인 창 레이아웃 설정 및 데이터 바인딩을 담당하는 클래스
 */
class MainWindow : public QMainWindow {
    Q_OBJECT // Qt의 시그널/슬롯 메커니즘을 위한 매크로

private:
    FlightGraph graph;            // 항공망 데이터 객체
    MapWidget* mapWidget;         // 커스텀 지도 위젯 포인터

    QLabel* airportTitleLabel;    // 선택된 공항 정보 타이틀
    QListWidget* departureList;   // 출발편 리스트
    QListWidget* arrivalList;     // 도착편 리스트

public:
    MainWindow() {
        setWindowTitle("국내선 항공권 조회 및 예매 프로그램");
        resize(1100, 780);
        setStyleSheet("background-color: #1e1e24; color: white; font-family: 'Malgun Gothic';");

        initGraphData();

        QWidget* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget);

        // [좌측 레이아웃 세팅]
        QVBoxLayout* leftLayout = new QVBoxLayout();
        QLabel* mainTitle = new QLabel("<h2>🛫 대한민국 국내선 운항 네트워크</h2>");
        leftLayout->addWidget(mainTitle);

        mapWidget = new MapWidget(graph, this);
        leftLayout->addWidget(mapWidget);

        // 버튼 매핑
        for (const auto& pair : graph.airports) {
            const Airport& ap = pair.second;
            QPushButton* apBtn = new QPushButton(QString::fromStdString(ap.code), mapWidget);
            apBtn->setFixedSize(45, 24);
            apBtn->setStyleSheet(
                "background-color: #0056b3; color: white; font-weight: bold; font-size: 10px;"
                "border: 1px solid white; border-radius: 4px;"
            );

            double sx = 560.0 / 819.0;
            double sy = 700.0 / 1024.0;
            int px = static_cast<int>(ap.x * sx);
            int py = static_cast<int>(ap.y * sy);
            apBtn->move(px - 22, py - 12);

            connect(apBtn, &QPushButton::clicked, this, [this, ap]() {
                loadAirportData(ap.code);
                });
        }

        QLabel* tipLabel = new QLabel("공항 노드를 클릭하면 실시간 출발/도착 국내 항공편이 동적 조회됩니다.");
        tipLabel->setStyleSheet("color: #aaaaaa; font-size: 11px;");
        leftLayout->addWidget(tipLabel);
        mainLayout->addLayout(leftLayout);

        // [우측 레이아웃 세팅]
        QVBoxLayout* rightLayout = new QVBoxLayout();
        rightLayout->setContentsMargins(15, 10, 15, 10);

        airportTitleLabel = new QLabel("<h3>공항 마커를 선택하세요</h3>"
            "<span style='color:#888;'>지도 위의 공항 단추를 누르면 타임테이블이 열립니다.</span>");
        rightLayout->addWidget(airportTitleLabel);

        rightLayout->addWidget(new QLabel("<b style='color:#5cb85c;'>🟢 해당 공항 출발 편 (Outbound)</b>"));
        departureList = new QListWidget();
        departureList->setStyleSheet("background-color: #2d2d35; border: 1px solid #444; padding: 5px; font-size: 12px;");
        rightLayout->addWidget(departureList);

        rightLayout->addWidget(new QLabel("<b style='color:#5bc0de;'>🔵 해당 공항 도착 편 (Inbound)</b>"));
        arrivalList = new QListWidget();
        arrivalList->setStyleSheet("background-color: #2d2d35; border: 1px solid #444; padding: 5px; font-size: 12px;");
        rightLayout->addWidget(arrivalList);

        mainLayout->addLayout(rightLayout, 1);

        connect(departureList, &QListWidget::itemClicked, this, &MainWindow::handleBooking);
        connect(arrivalList, &QListWidget::itemClicked, this, &MainWindow::handleBooking);
    }

private:
    void initGraphData() {
        graph.addAirport("ICN", "인천공항", 207, 220);
        graph.addAirport("GMP", "김포공항", 277, 220);
        graph.addAirport("CJJ", "청주공항", 373, 364);
        graph.addAirport("TAE", "대구공항", 547, 509);
        graph.addAirport("USN", "울산공항", 666, 551);
        graph.addAirport("PUS", "김해공항", 606, 651);
        graph.addAirport("KWJ", "광주공항", 277, 651);
        graph.addAirport("RSU", "여수공항", 389, 707);
        graph.addAirport("CJU", "제주공항", 297, 911);

        graph.addFlight("KE1201", "대한항공", "GMP", "CJU", 85000, "1시간 10분");
        graph.addFlight("OZ8901", "아시아나", "GMP", "CJU", 92000, "1시간 05분");
        graph.addFlight("7C103", "제주항공", "GMP", "CJU", 64000, "1시간 10분");
        graph.addFlight("KE1202", "대한항공", "CJU", "GMP", 88000, "1시간 10분");
        graph.addFlight("TW702", "티웨이항공", "CJU", "GMP", 59000, "1시간 05분");
        graph.addFlight("BX8101", "에어부산", "PUS", "CJU", 68000, "1시간 00분");
        graph.addFlight("LJ502", "진에어", "PUS", "CJU", 55000, "0시간 55분");
        graph.addFlight("BX8102", "에어부산", "CJU", "PUS", 71000, "1시간 00분");
        graph.addFlight("KE1103", "대한항공", "GMP", "PUS", 95000, "1시간 00분");
        graph.addFlight("BX8811", "에어부산", "GMP", "PUS", 78000, "0시간 55분");
        graph.addFlight("BX8812", "에어부산", "PUS", "GMP", 74000, "0시간 55분");
        graph.addFlight("ZE701", "이스타항공", "CJJ", "CJU", 58000, "1시간 05분");
        graph.addFlight("7C803", "제주항공", "CJJ", "CJU", 62000, "1시간 00분");
        graph.addFlight("ZE702", "이스타항공", "CJU", "CJJ", 61000, "1시간 05분");
        graph.addFlight("TW801", "티웨이항공", "TAE", "CJU", 57000, "1시간 00분");
        graph.addFlight("TW802", "티웨이항공", "CJU", "TAE", 59000, "1시간 00분");
        graph.addFlight("OZ8703", "아시아나", "KWJ", "CJU", 69000, "0시간 50분");
        graph.addFlight("OZ8704", "아시아나", "CJU", "KWJ", 72000, "0시간 50분");
        graph.addFlight("OZ8732", "아시아나", "RSU", "GMP", 83000, "0시간 55분");
        graph.addFlight("KE1401", "대한항공", "ICN", "CJU", 110000, "1시간 15분");
        graph.addFlight("KE1604", "대한항공", "USN", "GMP", 81000, "1시간 00분");
    }

    void addFlightToWidget(QListWidget* listWidget, const Flight& flight) {
        QString itemText = QString("[%1] %2 | %3 ➡️ %4 | %5원 | %6")
            .arg(QString::fromStdString(flight.flightNumber))
            .arg(QString::fromStdString(flight.airline))
            .arg(QString::fromStdString(flight.departure))
            .arg(QString::fromStdString(flight.destination))
            .arg(QLocale().toString(flight.price))
            .arg(QString::fromStdString(flight.duration));

        QListWidgetItem* item = new QListWidgetItem(itemText);
        item->setData(Qt::UserRole, QString::fromStdString(flight.flightNumber));
        item->setData(Qt::UserRole + 1, QString::fromStdString(flight.airline));
        item->setData(Qt::UserRole + 2, QString("%1 ➡️ %2").arg(QString::fromStdString(flight.departure)).arg(QString::fromStdString(flight.destination)));
        item->setData(Qt::UserRole + 3, QString("%1원").arg(QLocale().toString(flight.price)));
        item->setData(Qt::UserRole + 4, QString::fromStdString(flight.duration));
        listWidget->addItem(item);
    }

    void loadAirportData(const std::string& code) {
        mapWidget->setSelectedAirport(code);
        Airport ap = graph.airports[code];
        QString titleText = QString("<h2>%1 (%2)</h2><span style='color:#bbb;'>아래 항공권을 클릭하면 예매가 진행됩니다.</span>")
            .arg(QString::fromStdString(ap.name)).arg(QString::fromStdString(ap.code));
        airportTitleLabel->setText(titleText);

        departureList->clear();
        arrivalList->clear();

        if (graph.adjList.count(code)) {
            for (const auto& flight : graph.adjList[code]) {
                addFlightToWidget(departureList, flight);
            }
        }
        for (const auto& pair : graph.adjList) {
            for (const auto& flight : pair.second) {
                if (flight.destination == code) {
                    addFlightToWidget(arrivalList, flight);
                }
            }
        }
    }

    void handleBooking(QListWidgetItem* item) {
        if (!item) return;
        QString flightNum = item->data(Qt::UserRole).toString();
        QString airline = item->data(Qt::UserRole + 1).toString();
        QString route = item->data(Qt::UserRole + 2).toString();
        QString price = item->data(Qt::UserRole + 3).toString();
        QString duration = item->data(Qt::UserRole + 4).toString();

        QString message = QString("예매가 완료되었습니다.\n\n항공편: %1 (%2)\n구간: %3\n가격: %4\n소요 시간: %5")
            .arg(flightNum).arg(airline).arg(route).arg(price).arg(duration);
        QMessageBox::information(this, "예매 완료", message);
    }
};

#endif // MAINWINDOW_H