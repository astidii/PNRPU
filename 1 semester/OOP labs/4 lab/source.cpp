#pragma once
#include <iostream>
#include "Header.h"
using namespace std;


Student::Student() {
	this->name = "unknown";
	this->age = 0;
	this->subject = "unknuwn";
	this->mark = 0;
}

Student::Student(string name, int age, string subject, int mark) {
	this->name = name;
	this->age = age;
	this->subject = subject;
	this->mark = mark;
}

Student::Student(const Student& p) {
	this->name = p.name;
	this->age = p.age;
	this->subject = p.subject;
	this->mark = p.mark;
}

Student::~Student() {}

void Student::setName(string p) {
	this->name = p;
}

void Student::setAge(int p) {
	this->age = p;
}

void Student::setSubject(string p) {
	this->subject = p;
}

void Student::setMark(int p) {
	this->mark = p;
}

void Student::getInfo() {
	cout << "Имя: " << name << endl;
	cout << "Возраст: " << age << endl;
	cout << "Предмет: " << subject << endl;
	cout << "Оценка: " << mark << endl << endl;
}

ostream& operator<<(ostream& stream, const Student& p) {
	stream << p.name << endl;
	stream << p.age << endl;
	stream << p.subject << endl;
	if (p.mark < 3) {
		stream << p.mark << " (Неудовлитворительная оценка)" << endl;
	}
	else {
		stream << p.mark << endl;
	}
	cout << endl;
	return stream;
}

istream& operator>>(istream& stream, Student& p) {
	cout << "Введите имя: ";
	stream >> p.name;

	cout << "Введите возраст: ";
	stream >> p.age;

	cout << "Введите предмет: ";
	stream >> p.subject;

	cout << "Введите оценку: ";
	stream >> p.mark;

	cout << endl;

	return stream;
}