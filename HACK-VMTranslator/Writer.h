#pragma once
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class Writer
{
public:
	Writer(string filename);
	void writeArithmetic(string command);
	void writePushR(string origin);
	void writePopR(string origin);
	void writePushPop(const string command, string segment, int index);
	void writePush(string segment, int index);
	void writePop(string segment, int index);
	void writeBranching(string command, string name, int numArgs);
	void writeFunction(string functionName, int numArgs);
	void writeCall(string functionName, int numArgs);
	void writeReturn();
	void writeLabel(string label);
	void writeGoTo(string label);
	void writeIf(string label);
	void writeInit();
	void close();
private:
	ofstream fout;
	unsigned int labelCount = 0;

	string formatSegment(string segment);
};

