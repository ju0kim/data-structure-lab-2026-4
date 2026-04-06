#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Car.h"

using namespace std;

int main() {
	SportsCar myCar(100, "Ferrari", 3, false);

	cout << "-------------BASE STAT-------------" << endl;
	myCar.display();

	cout << "\n-------TURBO ON, SPEED UP---------" << endl;
	myCar.setTurbo(true);

	myCar.speedUp();
	myCar.display();

	cout << "\n--------THIS POINTER ADDRESS-------" << endl;
	myCar.whereAmI();
	
	return 0;
}