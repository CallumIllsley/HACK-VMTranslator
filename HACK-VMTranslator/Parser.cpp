#include "Parser.h"

Parser::Parser(string filename) {
	fin.open(filename);

	//if (fin.fail()) cout << filename << " failed to open." << endl;
	
	commandTypes["add"] = "C_ARITHMETIC";
	commandTypes["sub"] = "C_ARITHMETIC";
	commandTypes["neg"] = "C_ARITHMETIC";
	commandTypes["eq"] = "C_ARITHMETIC";
	commandTypes["gt"] = "C_ARITHMETIC";
	commandTypes["lt"] = "C_ARITHMETIC";
	commandTypes["and"] = "C_ARITHMETIC";
	commandTypes["or"] = "C_ARITHMETIC";
	commandTypes["not"] = "C_ARITHMETIC";
	commandTypes["pop"] = "C_POP";
	commandTypes["push"] = "C_PUSH";
	commandTypes["goto"] = "C_BRANCHING";
	commandTypes["if-goto"] = "C_BRANCHING";
	commandTypes["label"] = "C_BRANCHING";
	commandTypes["call"] = "C_BRANCHING";
	commandTypes["function"] = "C_BRANCHING";
	commandTypes["return"] = "C_BRANCHING";
}

bool Parser::hasMoreCommands() {
	return !fin.eof();
} 

void Parser::close() {
	fin.close();
}

void Parser::advance() {
	bool commandFound = false;
	string currentLine;
	unsigned long commentPos;

	while (!commandFound && getline(fin, currentLine)) {
		commentPos = currentLine.find("/");
		if (commentPos != string::npos) currentLine.erase(commentPos, currentLine.length() - commentPos);

		commandFound = !currentLine.empty();
	}
	
	currentCommand = currentLine;
	cout << currentLine << endl;
}

const string Parser::commandType() {
	string commandType = currentCommand.substr(0, currentCommand.find(" "));
	
	if (commandTypes.find(commandType) != commandTypes.end()) return commandTypes[commandType];
}

string Parser::command() {

	if (currentCommand.find(" ") != string::npos) {
		return currentCommand.substr(0, currentCommand.find(" "));
	}
	else return currentCommand;
}

string Parser::arg1() {
	vector<string> splitCommand;

	istringstream stream(currentCommand);
	copy(
		istream_iterator<string>(stream),
		istream_iterator<string>(),
		back_inserter(splitCommand)
	);
	if (!splitCommand.empty()) {
		if (splitCommand.size() > 1) {
			string arg1 = splitCommand.at(1);
			splitCommand.clear();
			return arg1;
		}
	}

	return "";
}

int Parser::arg2() {
	vector<string> splitCommand;

	istringstream stream(currentCommand);
	copy(
		istream_iterator<string>(stream),
		istream_iterator<string>(),
		back_inserter(splitCommand)
	);

	if (!splitCommand.empty()) {
		if (splitCommand.size() > 2) {
			string arg2 = splitCommand.at(2);
			splitCommand.clear();
			return stoi(arg2);
		}
	}
} 