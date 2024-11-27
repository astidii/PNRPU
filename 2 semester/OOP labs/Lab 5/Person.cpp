#include "Person.h"

Person::Person() {
	this->name = "unknown";
	this->age = 0;
}

Person::Person(string name, int age) {
	this->name = name;
	this->age = age;
}

Person::Person(const Person& p) {
	this->name = p.name;
	this->age = p.age;
}

Person::~Person() {}

void Person::setName(string p) {
	name = p;
}

void Person::setAge(int p) {
	age = p;
}

Person& Person::operator=(const Person& p)
{
	if (&p == this) { return *this; }
	else
	{
		name = p.name;
		age = p.age;
		return *this;
	}
}

istream& operator>>(istream& in, Person& p)
{
	cout << "Name: "; in >> p.name;
	cout << "Age: "; in >> p.age;
	return in;
}

ostream& operator<<(ostream& out, const Person& p)
{
	out << "Name: " << p.name << endl;
	out << "Age: " << p.age << endl;
	return out;
}

void Person::Show()
{
	cout << name << endl;
	cout << age << endl;
}
