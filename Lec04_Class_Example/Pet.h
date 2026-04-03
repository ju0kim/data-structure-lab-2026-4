#pragma once
#include<iostream>
#include <string>
using namespace std;

class Pet {
protected:
	string name;
	int age;
	string type;
public:
	//持失切
	Pet (string n,int a, string t)
		: name(n), age(a), type(t) {}

	//get name
	string getName() {
		return name;
	}

	//get age
	int getAge() {
		return age;
	}
	
	//get type
	string getType() {
		return type;
	}
};

class Puppy : public Pet {
private:
	string breed;

public:
	//持失切
	Puppy(string n, int a, string t, string b)
		: Pet(n, a, t), breed(b) {

	}
	//get breed
	string getBreed() {
		return breed;
	}
};