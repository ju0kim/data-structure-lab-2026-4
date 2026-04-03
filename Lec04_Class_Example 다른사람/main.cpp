#include "Rectangle.h"
#include "Pet.h"
#include "Music.h"

using namespace std;
int main() {
	Rectangle rect1(4, 6);
	cout << "Rectangle 1: " << endl;
	// printf("Rectangle 1:\n");
	cout << "Area: " << rect1.getArea() << endl;
	// printf("Area: %lf\n", rect1.getArea());
	cout << "Perimeter: " << rect1.getPerimeter() << endl;
	cout << "Is Square? : " << boolalpha << rect1.isSquare() << endl;
	printf("=========================================================\n");

	Pet myPet("Lucky", 5, "dog");
	cout << "My pet's name is " << myPet.getName() << endl;
	Puppy myPuppy("Kong", 3, "dog", "Puddle");
	cout << "My puppy's breed is " << myPuppy.getBreed() << endl;
	printf("=========================================================\n");

	// Creat a new music streaming service
	MusicStreamingService myService("Melon");
	// Add some music to the service
	myService.addMusic("Jump", "Blackpink", "Jump", 2025);
	myService.addMusic("Swim", "BTS", "Arirang", 2026);
	myService.addMusic("Bang Bang", "IVE", "Revive", 2026);
	myService.addMusic("404", "KiiKii", "Delulu", 2026);

	// Search by title
	string music_title;
	cout << "Enter the Music Title: ";
	cin >> music_title;
	Music* result = myService.searchByTitle(music_title);
	if (result != NULL) {
		cout << "Found: " << result->getTitle() << endl;
	}
	else {
		cout << "Not Found" << endl;
	}

	return 0;
}