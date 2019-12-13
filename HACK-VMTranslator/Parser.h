#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include <map>
#include <vector>
#include <sstream>
#include <iterator>

using namespace std;

class Parser
{
public:
	Parser(string filename);

	bool hasMoreCommands();
	void advance();
	const string commandType();
	string command();
	string arg1();
	int arg2();
	void close();

private: 
	string currentCommand;
	ifstream fin;
	map<string, string> commandTypes;
};

