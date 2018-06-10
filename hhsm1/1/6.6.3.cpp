/* 习题 7.6.3（输入/输出）
   题目要求: 
1. 读入一系列用空白字符分隔的 name-value 数据对；
2. 计算并打印每个 name 对应的全部 value 之和、平均数；
3. 计算并打印所有 name 对应的全部 value 之和、平均数；
4. 输入数据可从标准输入设备读取，亦可从指定文件读取；
5. 数据输出即可为标准输出设备，亦可为指定文件。
*/

#include <iostream>
#include <string>
#include <cstring>
#include <map>
#include <fstream>
using namespace std;

// Global variables 
struct T_NameState
{
	double currentSum;					
	unsigned int occus;     		//times 
	T_NameState() : currentSum(0), occus(0){ }
};
map<string, T_NameState> table;
istream* inputFile = &cin;
ostream* outputFile = &cout;

// clare 
int parseCommandLine(int argc, char* argv[]);
void readInput();
void updatePair(string name, double value);
void showResult();
void closeFile();

int main(int argc, char* argv[]) {
	--argc; ++argv;
	if (parseCommandLine(argc, argv) < 0) 
		{return 1;}

	readInput();
	showResult();
	closeFile();
	return 0;
}
int parseCommandLine(int argc, char* argv[]) {
	if (argc%2 != 0)
		{cerr <<"Invalidate arguments!"<<endl; return -1;}

	int i = 0;
	string argInputFileName, argOutputFileName;
	while (argc) {
		if (strcmp(argv[i], "-I") == 0) {
			argInputFileName = argv[i+1];
			i += 2; argc -= 2;
		}
		else if (strcmp(argv[i], "-O") == 0) {
			argOutputFileName = argv[i+1];
			i += 2; argc -= 2;
		}
		else {
			cerr <<"Unknown argument:\""<< argv[i] <<"\""<<endl;
			return -1;
		}
	}

	if (!argInputFileName.empty()) {
		inputFile = new ifstream(argInputFileName.c_str());
	}
	else 
		{inputFile = &cin;}

	if (!argOutputFileName.empty()) {
		outputFile = new ofstream(argOutputFileName.c_str());
	}
	else
		{outputFile = &cout;}

	if (inputFile == NULL || !(*inputFile)) {
		cerr <<"Can't open file:'"<< argInputFileName <<"'!"<<endl;
		return -2;
	}
	if (outputFile == NULL || !(*outputFile)) {
		cerr <<"Can't open file:'"<< argOutputFileName <<"'!"<<endl;
		return -2;
	}
}

void readInput() {
	string name; 	
	double value;

	while (*inputFile) {
		(*inputFile) >> name;
		if (!(*inputFile)) break;

		(*inputFile) >> value;
		if (!(*inputFile)) break;

		updatePair(name, value);
	}
}
void updatePair(string name, double value) {
	T_NameState& v = table[name];

	v.currentSum += value;
	++(v.occus);
}
void showResult() {
	map<string, T_NameState>::iterator itr = table.begin();
	double allSum = 0;

	for (; itr != table.end(); ++itr) {
		cout << itr->first <<" sum="
				 << (*itr).second.currentSum <<" ave="
				 << (*itr).second.currentSum / (*itr).second.occus
				 <<endl;
		allSum += (*itr).second.currentSum;
	}
	cout <<"Total Sum = "<< allSum <<endl
			 <<"Average = " << allSum / table.size() <<endl;
}
void closeFile() {
	if (inputFile != &cin) delete inputFile;
	if (outputFile != &cout) delete outputFile;
}
