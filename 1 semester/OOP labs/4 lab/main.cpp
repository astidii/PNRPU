#include <iostream>
#include <string>
#include "Header.h"
using namespace std;

int main() {
	system("chcp 1251 > Null");

	Student A1;
	cout << "Данные студента A1: " << endl;
	cin >> A1;

	Student B2;
	cout << "B2: " << endl << B2;

	B2 = A1;
	cout << "B2: " << endl << B2;

	B2.getInfo();




	return 0;
}