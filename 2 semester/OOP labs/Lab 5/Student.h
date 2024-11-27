#pragma once
#include "Person.h"

class Student : public Person
{
protected:
	string subject;
	int mark;

public:
	Student();
	Student(string, int, string, int);
	Student(const Student&);
	~Student();

	void setSubject(string);
	void setMark(int);

	string getSub() { return subject; }
	int getMark() { return mark; }

	Student& operator=(const Student&);
	friend ostream& operator<<(ostream&, const Student&);
	friend istream& operator>>(istream&, Student&);
	void Show();
};
