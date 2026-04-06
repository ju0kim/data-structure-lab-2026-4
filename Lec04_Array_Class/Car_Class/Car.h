#define _CRT_SECURE_NO_WARNINGS
#pragma once

#include <string.h>
#include <stdio.h>

using namespace std;

class Car {
protected:
	int speed;
	char name[40];
public:
	int gear;
	Car() {}
	~Car() {}

	Car(int s, const char* n, int g) : speed(s), gear(g) {
		strcpy(name, n);
	}

	void changeGear(int g = 4) {
		gear = g;
	}

	void speedUp() {
		speed += 5;
	}

	void display() {
		printf("[%s] : GEAR=%d SPEED=%dkmph \n", name, gear, speed);
	}

	void whereAmI() {
		printf("OBJECT ADDRESS=%p \n", this);
	}
};

class SportsCar : public Car {
public:
	bool bTurbo;

	SportsCar(int s, const char* n, int g, bool bTur = false) : Car(s, n, g) {
		bTurbo = bTur;
	}
	void setTurbo(bool bTur) {
		bTurbo = bTur;
	}

	void speedUp() {
		if (bTurbo) speed += 20;
		else Car::speedUp();
	}
};
