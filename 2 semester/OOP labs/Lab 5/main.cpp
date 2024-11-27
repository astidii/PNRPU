#include "Object.h"
#include "Person.h"
#include "Student.h"
#include "Vector.h"
#include <string>
#include <iostream>
using namespace std;

int main()
{
	cout << "Input per1: " << endl;
	Person per1;
	cin >> per1;

	cout << "Output: " << endl;
	cout << per1 << endl << endl;

	cout << "x->Show(): " << endl;
	Object* x = &per1;
	x->Show();

	cout << endl << endl;;

	cout << "per2 = per1: " << endl;
	Person per2;
	per2 = per1;

	cout << per2;

	cout << "Input s1(Student): " << endl;
	Student s1;
	cin >> s1;

	cout << endl;

	cout << s1;

	Object* s2 = &s1;
	s2->Show();
	cout << endl;


	Vector v(5);

	Person a;
	cin >> a;

	Student b;
	cin >> b;

	Object* p = &a;
	v.Add(p);
	p = &b;
	v.Add(p);
	cout << v;

	return 0;
}
