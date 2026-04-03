#include"Rectangle.h"
#include "Pet.h"
#include "Music.h"
using namespace std;
int main() {
	//Rectangle1 객체생성
	Rectangle rect1(3.4, 2.5);
	cout << "Rectangle 1:" << endl;
	//이거랑 똑같음 printf("Rectangle 1:\n");
	cout << "Area:" << rect1.getArea() << endl;
	//printf("Area: %lf", rect1.getArea());
	cout << "Perimeter:" << rect1.getPerimeter() << endl;
	cout << "Is Square?" << boolalpha << rect1.isSquare() << endl;
	printf("=================================================================\n");



	// PET CLASS 테스트
	Pet myPet("Kong", 5, "Dog");
	cout << "My pet's name is " << myPet.getName() << endl;

	// puppy 테스트
	Puppy myPuppy("Lucky", 10, "dog", "poodle");
	cout << "My puppy's breed is " << myPuppy.getBreed() << endl;

	return 0;

	printf("=======================================================\n");

	//streaming test

	MusicStreamingService myService("Spotify");

	//음악 service에 추가하기
	myService.addMusic("SWIM", "BTS", "ARIRANG", 2026);
	myService.addMusic("RUDE", "Hearts2Hearts", "RUDE", 2026);
	myService.addMusic("BANGBANG", "IVE", "REVIVE", 2026);
	myService.addMusic("JUMP", "BlackPink", "jump", 2025);
	myService.addMusic("ShutDown", "BlackPink", "BornPink", 2022);
	//TITLE로 SEARCH
	string music_title;
	cout << "Enter theMusic Title:";
	cin >> music_title;
	//scanf_s("%s",&music_title); 이거랑 똑같음
	Music* reasult = myService.searchByTItle(music_title);
	if (result != NULL) {
		cout << "Found:" << result->getTilte() << "by " << result->getArtis() << endl;
		//gettitle 앞에만 화살표인 이유 제미나이 질문
		

	}
	else {
		cout << "Not Found" << endl;
	}
};
