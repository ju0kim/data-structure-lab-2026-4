#include <QApplication>
#include "MainWindow.h" // 우리가 분리해낸 메인 윈도우 헤더

int main(int argc, char* argv[]) {
    QApplication app(argc, argv); // Qt 자원 및 이벤트 루프 초기화

    MainWindow window;            // 객체지향화된 메인 윈도우 인스턴스 생성
    window.show();                // 화면에 띄우기

    return app.exec();            // 루프 가동 및 프로그램 제어권 전환
}