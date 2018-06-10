#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <fstream>
using namespace std;

double v[10000];

int main() {
	int i = 0;
	ifstream iif("0_1.csv");
	while (iif.eof()!=0) {
		int a;
		string b;
		double c, d, e;
		iif >>a>>b>>c>>d>>e>> v[i];
		i++;
	}
	
}