#include <iostream>
#include <cstdio>
using namespace std;

char* itoa(int a, char b[]) {
	sprintf(b, "%d", a);
	return b;
}

int main() {
	int a;
	char b[20];
	cin >> a;
	itoa(a, b);
	cout << b <<endl;
	return 0;
}

