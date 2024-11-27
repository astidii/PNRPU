#pragma once
#include "object.h"
#include <iostream>
#include <string>
using namespace std;

class Vector
{
private:
	Object** beg;
	int size;
	int cur;

public:
	Vector();
	Vector(int);
	~Vector();

	void Add(Object*);
	friend ostream& operator<<(ostream& out, const Vector&);

};
