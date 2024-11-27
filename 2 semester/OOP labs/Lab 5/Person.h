#pragma once
#include "object.h"
#include <iostream>
#include <string>
using namespace std;


class Person : public Object
{
protected:
	string name;
	int age;

public:
	Person();
	Person(string, int);
	Person(const Person&);
	virtual ~Person();

	void setName(string);
	void setAge(int);

	string getName() { return name; }
	int getAge() { return age; }

	Person& operator=(const Person&);
	friend istream& operator>>(istream& in, Person&);
	friend ostream& operator<<(ostream& out, const Person&);
	void Show();
};
