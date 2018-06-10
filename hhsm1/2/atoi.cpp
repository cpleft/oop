#include <iostream>
#include <cstdio>
#include <cstring>
using namespace std;

int atoi(const char* s);
int d(const char* s, int i, int sig);
int h(const char* s, int i, int sig);
int o(const char* s, int i, int sig);

char some_effectC[] = {
	'1', '2', '3', '4', '5', '6', '7', '8', '9',
	'0', '+', '-'
};
char hh[] = {
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'A', 'B', 'C', 'D', 'E', 'F'
};

int main() {
	char s[1000];
	scanf("%s", s);
	cout << atoi(s) <<endl;
	return 0;
}

int atoi(const char* s) {
	int i = 0;
	int sig = 0;  // 0:+, 1:-
	while (s[i] != '\0') {
		// blank?
		if (s[i]=='\n' || s[i]=='\t' || s[i]==' ')
			{ i++; continue;}

		// effective?
		int flag = 0;
		for (int j = 0; j < 12; j++) 
			if (s[i] == some_effectC[j]) flag = 1;
		if (flag == 0) return 0;

		// + -
		if (s[i] == '+') { i++; continue;}
		if (s[i] == '-') { sig = 1; i++; continue;}

		// 1-9, 0, 0x
		if (s[i] == '0') {
			if (s[i+1] == 'x') return h(s, i+2, sig);

			int flag = 0;
			for (int j = 0; j <= 9; j++) 
				if (s[i+1] == (char)(j+'0')) flag = 1;
			if (flag == 1) return o(s, i+1, sig);
			else return 0;
		}
		return d(s, i, sig);
	}
	return 0;
}

int d(const char* s, int i, int sig) {
	int t = 0;
	while (s[i] != '\0') {
		// effective?
		int flag = 0;
		for (int j = 0; j <= 9; j++) 
			if (s[i] == (char)(j+'0')) flag = 1;
		if (flag == 0) break;

		t = 10*t+(int)(s[i]-'0');
		i++;
	}
	if (sig == 0)
		return t;
	if (sig == 1)
		return -t;
}
int o(const char* s, int i, int sig) {
	int t = 0;
	while (s[i] != '\0') {
		// effective?
		int flag = 0;
		for (int j = 0; j <= 7; j++) 
			if (s[i] == (char)(j+'0')) flag = 1;
		if (flag == 0) break;

		t = 8*t+(int)(s[i]-'0');
		i++;
	}
	if (sig == 0)
		return t;
	if (sig == 1)
		return -t;
}
int h(const char* s, int i, int sig) {
	int t = 0;
	while (s[i] != '\0') {
		// effective?
		int flag = 0;
		for (int j = 1; j <= 16; j++) 
			if (s[i] == hh[j]) flag = 1;
		if (flag == 0) break;

		int tmp;
		if (s[i]=='A'||s[i]=='B'||s[i]=='C'||s[i]=='D'
			||s[i]=='E'||s[i]=='F')
			tmp = (int)(s[i]-'A'+10);
		else tmp = (int)(s[i]-'0');
		t = 16*t+tmp;
		i++;
	}
	if (sig == 0)
		return t;
	if (sig == 1)
		return -t;
}