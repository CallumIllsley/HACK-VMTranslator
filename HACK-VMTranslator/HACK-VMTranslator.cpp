#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#include <iostream>
#include "Parser.h"
#include "Writer.h"
#include <vector>
#include <experimental/filesystem>

namespace fs = experimental::filesystem;
using namespace std;

string genOutPath(string in) {
	return in;
}

string genOutPathDir(string in) {
	string out = in.substr(0, in.length() - 1);
	out = out.substr(out.find_last_of("/") + 1, out.length()) + ".asm"; 
	return out;
}

int main()
{
	string dir; 
	string outFilename;

	cout << "Enter name of file/directory to translate:" << endl;
	cin >> dir;
	dir = dir + "/";

	if (dir.find("/") != string::npos) outFilename = genOutPathDir(dir);
	else outFilename = genOutPath(dir);

	Writer writer(outFilename);

	writer.writeInit();

	for (const auto& entry : fs::recursive_directory_iterator(dir)) {
		ostringstream oss;
		oss << entry;
		string file = oss.str();

		if (file.find(".vm") != string::npos) {
			Parser parser(file);
			string command;

			while (true) {
				parser.advance();
				command = parser.command();
				if (!parser.hasMoreCommands()) break;
				if (parser.commandType() == "C_ARITHMETIC") writer.writeArithmetic(parser.command());
				if (parser.commandType() == "C_BRANCHING") writer.writeBranching(parser.command(), parser.arg1(), parser.arg2());
				if (parser.commandType() == "C_POP") writer.writePushPop("C_POP", parser.arg1(), parser.arg2());
				if (parser.commandType() == "C_PUSH") writer.writePushPop("C_PUSH", parser.arg1(), parser.arg2());
			}
		}
		else continue;	
	}

	writer.close();
}



