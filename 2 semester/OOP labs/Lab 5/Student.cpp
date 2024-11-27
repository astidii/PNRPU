#include "Student.h"

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

void Student::setSubject(string p) {
	subject = p;
}

void Student::setMark(int p) {
	mark = p;
}

void Student::Show() {
	cout << "Name: " << name << endl;
	cout << "Age: " << age << endl;
	cout << "Subject: " << subject << endl;
	cout << "Mark: " << mark << endl << endl;
}

Student& Student::operator=(const Student& p)
{
	if (&p == this) { return *this; }
	else
	{
		name = p.name;
		age = p.age;
		subject = p.subject;
		mark = p.mark;
		return *this;
	}
}

ostream& operator<<(ostream& stream, const Student& p) {
	stream << p.name << endl;
	stream << p.age << endl;
	stream << p.subject << endl;
	if (p.mark < 3) {
		stream << p.mark << " (So bad, bro)" << endl;
	}
	else {
		stream << p.mark << endl;
	}
	cout << endl;
	return stream;
}

istream& operator>>(istream& stream, Student& p) {
	cout << "Name: ";
	stream >> p.name;

	cout << "Age: ";
	stream >> p.age;

	cout << "Subject: ";
	stream >> p.subject;

	cout << "Mark: ";
	stream >> p.mark;

	cout << endl;

	return stream;
}
