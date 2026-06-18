#include <QApplication>
#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QListWidget>
#include <QMessageBox>
#include <QPixmap>
#include <QPainter>
#include <QPen>
#include <QLocale>
#include <vector>
#include <string>
#include <map>
#include <QListWidgetItem>

// ==========================================
// 1. 그래프 자료구조 및 데이터 정의 영역
// ==========================================

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
    int x;                     // 원본 지도 이미지(819x1024) 상의 X 좌표
    int y;                     // 원본 지도 이미지(819x1024) 상의 Y 좌표
};

/**
 * @brief 항공 네트워크를 관리하는 그래프 클래스 (인접 리스트 구조)
 */
class FlightGraph {
public:
    // 공항 코드를 Key로 사용하여 공항 상세 정보를 저장하는 Map (정점 집합)
    std::map<std::string, Airport> airports;
    
    // 출발 공항 코드를 Key로, 해당 공항에서 출발하는 항공편들의 목록을 Value로 가지는 인접 리스트
    // 시간 복잡도와 메모리 효율성을 모두 잡은 직관적인 그래프 구현 방식입니다.
    std::map<std::string, std::vector<Flight>> adjList; 

    /**
     * @brief 그래프에 공항(정점)을 추가하는 메서드
     */
    void addAirport(const std::string& code, const std::string& name, int x, int y) {
        airports[code] = Airport{code, name, x, y};
    }

    /**
     * @brief 그래프에 항공편(단방향 간선)을 추가하는 메서드
     */
    void addFlight(const std::string& num, const std::string& airline, 
                   const std::string& from, const std::string& to, int price, const std::string& duration) {
        adjList[from].push_back(Flight{num, airline, from, to, price, duration});
    }
};


// ==========================================
// 2. 대한민국 국내선 지도 및 노선 렌더링 위젯 영역
// ==========================================

/**
 * @brief 지도 이미지를 배경으로 깔고, 공항 간 노선을 화면에 직접 그리는 커스텀 위젯
 */
class MapWidget : public QLabel {
private:
    FlightGraph& graph;             // 메인 윈도우에서 관리하는 그래프 데이터의 참조 변수
    std::string selectedAirport;    // 현재 사용자가 클릭하여 선택한 공항의 코드

    // 지도 데이터의 정밀한 좌표 매핑을 위한 원본 이미지 해상도 기준 정의
    const int ORIGINAL_MAP_WIDTH = 819;
    const int ORIGINAL_MAP_HEIGHT = 1024;

    /**
     * @brief 원본 이미지 좌표를 현재 창 크기에 맞게 비율을 계산하여 변환해주는 헬퍼 메서드
     */
    QPoint scalePoint(int x, int y) const {
        // 현재 위젯의 실제 가로/세로 길이를 원본 크기로 나누어 비율(Ratio) 계산
        double sx = (double)width() / ORIGINAL_MAP_WIDTH;
        double sy = (double)height() / ORIGINAL_MAP_HEIGHT;

        // 계산된 비율을 곱한 후 정수형 픽셀 좌표(QPoint)로 반환
        return QPoint(
            static_cast<int>(x * sx),
            static_cast<int>(y * sy)
        );
    }

public:
    MapWidget(FlightGraph& g, QWidget* parent = nullptr) : QLabel(parent), graph(g) {
        setFixedSize(560, 700);       // UI 배치 안정을 위해 지도 위젯 크기 고정
        setAlignment(Qt::AlignCenter); // 텍스트 혹은 에러 메시지 출력 시 중앙 정렬
        
        QPixmap pixmap("map.png");    // 실행 파일과 같은 경로에 있는 map.png 로드
        if (pixmap.isNull()) {
            // 이미지 로드 실패 시, 사용자가 인지할 수 있도록 에러 가이드라인 표시
            setText("<h3 style='color:gray;'>[map.png 지도를 찾을 수 없습니다]<br>"
                    "좌측 탐색기에 map.png 파일을 정확히 업로드해주세요.</h3>");
            setStyleSheet("background-color: #222226; border: 1px solid #444;");
        } else {
            // 부드러운 필터링(SmoothTransformation)을 적용하여 고정 크기로 이미지 스케일링 후 부착
            setPixmap(pixmap.scaled(560, 700, Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
        }
    }

    /**
     * @brief 사용자가 공항을 선택했을 때 호출되어 강조할 노선을 갱신하는 메서드
     */
    void setSelectedAirport(const std::string& code) {
        selectedAirport = code;
        update(); // 위젯을 다시 그리도록 강제 유도하여 paintEvent()를 실행시킴
    }

protected:
    /**
     * @brief QLabel의 그리기 기능을 확장하여 항공 노선(선)을 그리는 페인트 이벤트
     */
    void paintEvent(QPaintEvent* event) override {
        QLabel::paintEvent(event); // 부모 클래스의 paintEvent를 먼저 호출하여 배경(지도 이미지)을 먼저 그림
        Q_UNUSED(event);           // 사용하지 않는 매개변수에 대한 컴파일러 경고 방지

        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing); // 선을 부드럽게 그리도록 계단현상 방지 활성화

        // [인접 리스트 이중 루프 순회] 그래프 내의 모든 출발 공항을 순회
        for (const auto& pair : graph.adjList) {
            // 해당 출발 공항에서 출발하는 모든 항공편(간선)을 순회
            for (const auto& flight : pair.second) {
                // 출발 공항과 도착 공항이 실제로 등록된 공항인지 검색
                auto fromIt = graph.airports.find(flight.departure);
                auto toIt = graph.airports.find(flight.destination);

                // 둘 중 하나라도 정상적인 정점이 아니라면 예외 처리 (건너뛰기)
                if (fromIt == graph.airports.end() || toIt == graph.airports.end())
                    continue;

                const Airport& fromVertex = fromIt->second;
                const Airport& toVertex = toIt->second;

                // 좌표 값이 비정상적(0 이하)인 경우 렌더링에서 제외
                if (fromVertex.x <= 0 || fromVertex.y <= 0 || toVertex.x <= 0 || toVertex.y <= 0)
                    continue;

                // [조건부 스타일링] 현재 그려지는 노선이 사용자가 선택한 공항과 연관되어 있는지 확인
                if (flight.departure == selectedAirport || flight.destination == selectedAirport) {
                    painter.setPen(QPen(QColor(255, 215, 0), 3)); // 선택된 공항의 활성화 노선은 굵은 노란색 실선
                } else {
                    painter.setPen(QPen(QColor(0, 86, 179, 60), 1, Qt::DotLine)); // 일반 노선은 은은한 투명도의 파란 점선
                }

                // 지도 해상도 비례 좌표 계산 수행
                QPoint p1 = scalePoint(fromVertex.x, fromVertex.y);
                QPoint p2 = scalePoint(toVertex.x, toVertex.y);

                // 두 공항의 좌표를 연결하는 항공 경로선 렌더링
                painter.drawLine(p1, p2);
            }
        }
    }
};


// ==========================================
// 3. 메인 윈도우 UI 및 핵심 비즈니스 로직 영역
// ==========================================

/**
 * @brief 애플리케이션의 메인 창 레이아웃 설정 및 데이터 바인딩을 담당하는 클래스
 */
class MainWindow : public QMainWindow {
private:
    FlightGraph graph;            // 프로그램 전체에서 공유 및 관리할 항공망 그래프 객체
    MapWidget* mapWidget;         // 커스텀 지도 위젯 포인터
    
    QLabel* airportTitleLabel;    // 선택된 공항 이름 및 타이틀을 표시할 라벨
    QListWidget* departureList;   // 선택된 공항에서 출발하는 항공편 리스트 위젯 (Outbound)
    QListWidget* arrivalList;     // 선택된 공항으로 들어오는 항공편 리스트 위젯 (Inbound)

public:
    MainWindow() {
        setWindowTitle("국내선 항공권 조회 및 예매 프로그램 ");
        resize(1100, 780); 
        // 다크 모드 테마 스킬셋 정의 및 가독성 높은 맑은 고딕 폰트 지정
        setStyleSheet("background-color: #1e1e24; color: white; font-family: 'Malgun Gothic';");

        initGraphData(); // 그래프 초기 데이터 주입 함수 호출

        // 메인 윈도우의 중심 역할을 할 센트럴 위젯 생성 및 레이아웃 설정
        QWidget* centralWidget = new QWidget(this);
        setCentralWidget(centralWidget);
        QHBoxLayout* mainLayout = new QHBoxLayout(centralWidget); // 가로 배치 레이아웃 (좌: 지도, 우: 정보창)

        // ------------------------------------------
        // [좌측 영역] 대한민국 국내선 지도 및 노드 버튼 배치
        // ------------------------------------------
        QVBoxLayout* leftLayout = new QVBoxLayout();
        QLabel* mainTitle = new QLabel("<h2>🛫 대한민국 국내선 운항 네트워크</h2>");
        leftLayout->addWidget(mainTitle);

        mapWidget = new MapWidget(graph, this);
        leftLayout->addWidget(mapWidget);

        // 그래프에 등록된 공항 정점 리스트를 순회하며 지도 위에 인터랙티브한 QPushButton 매핑
        for (const auto& pair : graph.airports) {
            const Airport& ap = pair.second;
            // 공항 코드(예: ICN)를 텍스트로 가지는 버튼 생성 및 지도 위젯을 부모로 지정
            QPushButton* apBtn = new QPushButton(QString::fromStdString(ap.code), mapWidget);
            apBtn->setFixedSize(45, 24);
            apBtn->setStyleSheet(
                "background-color: #0056b3; color: white; font-weight: bold; font-size: 10px;"
                "border: 1px solid white; border-radius: 4px;"
            );
            
            // 지도 위젯의 현재 스케일에 맞춰 공항 버튼이 위치할 실제 픽셀 좌표 환산
            double sx = 560.0 / 819.0;
            double sy = 700.0 / 1024.0;

            int px = static_cast<int>(ap.x * sx);
            int py = static_cast<int>(ap.y * sy);

            // 버튼의 중심점이 공항 좌표 정중앙에 정확히 일치하도록 오프셋 분할 계산 적용 (가로 반, 세로 반 차감)
            apBtn->move(px - 22, py - 12); 

            // 람다(Lambda) 식을 활용하여 버튼 클릭 시 해당 공항의 데이터를 로드하는 이벤트 슬롯 연결
            connect(apBtn, &QPushButton::clicked, this, [this, ap]() {
                loadAirportData(ap.code);
            });
        }
        
        QLabel* tipLabel = new QLabel("공항 노드를 클릭하면 실시간 출발/도착 국내 항공편이 동적 조회됩니다.");
        tipLabel->setStyleSheet("color: #aaaaaa; font-size: 11px;");
        leftLayout->addWidget(tipLabel);
        mainLayout->addLayout(leftLayout); // 왼쪽에 지도 레이아웃 배치 완료

        // ------------------------------------------
        // [우측 영역] 타임테이블 정보창 및 예매 인터페이스 영역
        // ------------------------------------------
        QVBoxLayout* rightLayout = new QVBoxLayout();
        rightLayout->setContentsMargins(15, 10, 15, 10);

        // 상단 안내 타이틀 라벨 초기화
        airportTitleLabel = new QLabel("<h3>공항 마커를 선택하세요</h3>"
                                       "<span style='color:#888;'>지도 위의 공항 단추를 누르면 타임테이블이 열립니다.</span>");
        rightLayout->addWidget(airportTitleLabel);

        // 출발편 세션 구축
        rightLayout->addWidget(new QLabel("<b style='color:#5cb85c;'>🟢 해당 공항 출발 편 (Outbound)</b>"));
        departureList = new QListWidget();
        departureList->setStyleSheet("background-color: #2d2d35; border: 1px solid #444; padding: 5px; font-size: 12px;");
        rightLayout->addWidget(departureList);

        // 도착편 세션 구축
        rightLayout->addWidget(new QLabel("<b style='color:#5bc0de;'>🔵 해당 공항 도착 편 (Inbound)</b>"));
        arrivalList = new QListWidget();
        arrivalList->setStyleSheet("background-color: #2d2d35; border: 1px solid #444; padding: 5px; font-size: 12px;");
        rightLayout->addWidget(arrivalList);

        mainLayout->addLayout(rightLayout, 1); // 우측 레이아웃 배치 완료 (우측 공간이 늘어나도록 가중치 1 부여)

        // 리스트 항목(항공권)을 마우스로 클릭했을 때 예매 프로세스(handleBooking)가 작동하도록 슬롯 연결
        connect(departureList, &QListWidget::itemClicked, this, &MainWindow::handleBooking);
        connect(arrivalList, &QListWidget::itemClicked, this, &MainWindow::handleBooking);
    }

private:
    /**
     * @brief 대한민국 주요 공항(Vertex) 및 노선(Edge) 기초 데이터 삽입 메서드
     */
    void initGraphData() {
        // 공항 코드, 공항명, 원본 819x1024 지도상의 X, Y 픽셀 좌표 등록
        graph.addAirport("ICN", "인천공항", 207, 220);
        graph.addAirport("GMP", "김포공항", 277, 220);
        graph.addAirport("CJJ", "청주공항", 373, 364);
        graph.addAirport("TAE", "대구공항", 547, 509);
        graph.addAirport("USN", "울산공항", 666, 551);
        graph.addAirport("PUS", "김해공항", 606, 651);
        graph.addAirport("KWJ", "광주공항", 277, 651);
        graph.addAirport("RSU", "여수공항", 389, 707);
        graph.addAirport("CJU", "제주공항", 297, 911);

        // 항공편명, 항공사, 출발지, 도착지, 가격, 소요시간 설정 규칙에 의거 단방향 노선 매핑
        // [김포 - 제주 노선]
        graph.addFlight("KE1201", "대한항공", "GMP", "CJU", 85000, "1시간 10분");
        graph.addFlight("OZ8901", "아시아나", "GMP", "CJU", 92000, "1시간 05분");
        graph.addFlight("7C103",   "제주항공", "GMP", "CJU", 64000, "1시간 10분");
        graph.addFlight("KE1202", "대한항공", "CJU", "GMP", 88000, "1시간 10분");
        graph.addFlight("TW702",   "티웨이항공", "CJU", "GMP", 59000, "1시간 05분"); 

        // [김해(부산) - 제주 노선]
        graph.addFlight("BX8101", "에어부산", "PUS", "CJU", 68000, "1시간 00분");
        graph.addFlight("LJ502",   "진에어",   "PUS", "CJU", 55000, "0시간 55분");
        graph.addFlight("BX8102", "에어부산", "CJU", "PUS", 71000, "1시간 00분"); 

        // [김포 - 김해(부산) 노선]
        graph.addFlight("KE1103", "대한항공", "GMP", "PUS", 95000, "1시간 00분");
        graph.addFlight("BX8811", "에어부산", "GMP", "PUS", 78000, "0시간 55분"); 
        graph.addFlight("BX8812", "에어부산", "PUS", "GMP", 74000, "0시간 55분");

        // [청주 - 제주 노선]
        graph.addFlight("ZE701",   "이스타항공", "CJJ", "CJU", 58000, "1시간 05분"); 
        graph.addFlight("7C803",   "제주항공", "CJJ", "CJU", 62000, "1시간 00분");
        graph.addFlight("ZE702",   "이스타항공", "CJU", "CJJ", 61000, "1시간 05분"); 

        // [대구 - 제주 노선]
        graph.addFlight("TW801",   "티웨이항공", "TAE", "CJU", 57000, "1시간 00분"); 
        graph.addFlight("TW802",   "티웨이항공", "CJU", "TAE", 59000, "1시간 00분"); 

        // [광주 - 제주 노선]
        graph.addFlight("OZ8703", "아시아나", "KWJ", "CJU", 69000, "0시간 50분");
        graph.addFlight("OZ8704", "아시아나", "CJU", "KWJ", 72000, "0시간 50분");

        // [여수 - 김포 노선]
        graph.addFlight("OZ8732", "아시아나", "RSU", "GMP", 83000, "0시간 55분");
        
        // [인천 - 제주 노선]
        graph.addFlight("KE1401", "대한항공", "ICN", "CJU", 110000, "1시간 15분");
        
        // [울산 - 김포 노선]
        graph.addFlight("KE1604", "대한항공", "USN", "GMP", 81000, "1시간 00분");
    }

    /**
     * @brief 중복 코드를 방지하고 QListWidget에 개별 항공권 아이템을 규격화하여 삽입하는 헬퍼 메서드
     * @details Qt::UserRole 데이터를 바인딩하여 뷰에 보이는 텍스트와 별개로 실제 비즈니스 데이터를 메타데이터로 숨겨놓습니다.
     */
    void addFlightToWidget(QListWidget* listWidget, const Flight& flight) {
        // 화폐 쉼표 표기(QLocale) 및 서식 지정을 사용한 문자열 조립
        QString itemText = QString("[%1] %2 | %3 ➡️ %4 | %5원 | %6")
                           .arg(QString::fromStdString(flight.flightNumber))
                           .arg(QString::fromStdString(flight.airline))
                           .arg(QString::fromStdString(flight.departure))
                           .arg(QString::fromStdString(flight.destination))
                           .arg(QLocale().toString(flight.price))
                           .arg(QString::fromStdString(flight.duration));

        QListWidgetItem* item = new QListWidgetItem(itemText);
        
        // [메타데이터 저장 고도화] 사용자가 아이템을 클릭했을 때 조회가 쉽도록 내부 저장공간(UserRole)에 파편화하여 기록
        item->setData(Qt::UserRole, QString::fromStdString(flight.flightNumber));
        item->setData(Qt::UserRole + 1, QString::fromStdString(flight.airline));
        item->setData(Qt::UserRole + 2, QString("%1 ➡️ %2").arg(QString::fromStdString(flight.departure)).arg(QString::fromStdString(flight.destination)));
        item->setData(Qt::UserRole + 3, QString("%1원").arg(QLocale().toString(flight.price)));
        item->setData(Qt::UserRole + 4, QString::fromStdString(flight.duration));
        
        listWidget->addItem(item); // 타겟 위젯 목록에 추가
    }

    /**
     * @brief 사용자가 특정 공항 노드를 클릭 시 인접 리스트 데이터를 탐색하여 양방향 타임테이블을 적재하는 핵심 함수
     */
    void loadAirportData(const std::string& code) {
        mapWidget->setSelectedAirport(code); // 지도 위젯에 선택 알림 -> 관련 간선 강조 및 리드로우 트리거
        
        Airport ap = graph.airports[code];
        QString titleText = QString("<h2>%1 (%2)</h2>"
                                    "<span style='color:#bbb;'>아래 항공권을 클릭하면 예매가 진행됩니다.</span>")
                            .arg(QString::fromStdString(ap.name))
                            .arg(QString::fromStdString(ap.code));
        airportTitleLabel->setText(titleText);

        // 기존에 로드되어 누적되어 있던 타임테이블 정보 완전 제거
        departureList->clear();
        arrivalList->clear();

        // 1. 출발편 (Outbound): 인접 리스트 특성상 해시 맵 검색 한 번으로 즉시 O(1) 접근 후 순회 및 추가
        if (graph.adjList.count(code)) {
            for (const auto& flight : graph.adjList[code]) {
                addFlightToWidget(departureList, flight);
            }
        }
        
        // 2. 도착편 (Inbound): 단방향 그래프의 인접 리스트 구조 특성상, 목적지가 동일한 역방향 간선을 찾기 위해
        // 그래프 전체 노선을 전수 스캔(Full Scan)하는 방식을 안전하게 수행합니다.
        for (const auto& pair : graph.adjList) {
            for (const auto& flight : pair.second) {
                if (flight.destination == code) {
                    addFlightToWidget(arrivalList, flight);
                }
            }
        }
    }

    /**
     * @brief 타임테이블 내 항공편 아이템 클릭 시 팝업창을 띄워 최종 예매 확인을 시켜주는 이벤트 핸들러 슬롯
     */
    void handleBooking(QListWidgetItem* item) {
        if (!item) return; // 잘못된 클릭 예외 처리

        // addFlightToWidget 메서드에서 숨겨두었던 메타데이터(UserRole)를 역순으로 안전하게 추출
        QString flightNum = item->data(Qt::UserRole).toString();
        QString airline = item->data(Qt::UserRole + 1).toString();
        QString route = item->data(Qt::UserRole + 2).toString();
        QString price = item->data(Qt::UserRole + 3).toString();
        QString duration = item->data(Qt::UserRole + 4).toString();

        // 사용자 친화적이고 직관적인 메시지 레이아웃 포맷팅
        QString message = QString(
            "예매가 완료되었습니다.\n\n"
            "항공편: %1 (%2)\n"
            "구간: %3\n"
            "가격: %4\n"
            "소요 시간: %5"
        ).arg(flightNum).arg(airline).arg(route).arg(price).arg(duration);

        // Qt 표준 정보 메시지 박스 다이얼로그 호출
        QMessageBox::information(this, "예매 완료", message);
    }
};

// ==========================================
// 4. 애플리케이션 진입점(Main)
// ==========================================
int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // Qt 어플리케이션 이벤트 루프 및 자원 초기화
    MainWindow window;            // 메인 윈도우 인스턴스 생성
    window.show();                // 화면 표시
    return app.exec();            // 루프 제어권을 Qt에 넘기며 어플리케이션 가동
}