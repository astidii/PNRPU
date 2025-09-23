#pragma once
#pragma once
#include <iostream>
using namespace std;

class Person {
protected:
	string name;
	int age;

};

class Student : public Person {
	string subject;
	int mark;

public:
	Student();
	Student(string, int, string, int);
	Student(const Student&);
	~Student();

	void setName(string);
	void setAge(int);
	void setSubject(string);
	void setMark(int);

	void getInfo();

	friend ostream& operator<<(ostream&, const Student&);
	friend istream& operator>>(istream&, Student&);

	Student& operator=(const Student& p) {
		this->name = p.name;
		this->age = p.age;
		this->subject = p.subject;
		this->mark = p.mark;
		return *this;
	}
};
