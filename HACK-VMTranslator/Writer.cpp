#include "Writer.h"

Writer::Writer(string filename) {
	fout.open(filename);

	if (fout.fail()) cout << filename << " failed to open." << endl;
}

//Writes arithmetic logic based on command
void Writer::writeArithmetic(string command) {
	fout << "@SP" << endl;
	fout << "A=M-1" << endl;

	if (command == "neg") {
		fout << "M=-M" << endl;
		return;
	}

	if (command == "not") {
		fout << "M=!M" << endl;
		return;
	}

	fout << "D=M" << endl;
	fout << "A=A-1" << endl;

	if (command == "add") {
		fout << "M=M+D" << endl;
	}

	if (command == "sub") {
		fout << "M=M-D" << endl;
	}

	if (command == "and") {
		fout << "M=D&M" << endl;
	}

	if (command == "or") {
		fout << "M=D|M" << endl;
	}

	if (command == "lt") {
		fout << "D=M-D" << endl;

		fout << "@ltTrue" << labelCount << endl;
		fout << "D;JLT" << endl;
		fout << "D=0" << endl;
		fout << "@ltDone" << labelCount << endl;
		fout << "0;JMP" << endl;

		fout << "(ltTrue" << labelCount << ")" << endl;
		fout << "D=-1" << endl;

		fout << "(ltDone" << labelCount << ")" << endl;
		fout << "@SP" << endl;
		fout << "A=M-1" << endl;
		fout << "A=A-1" << endl;
		fout << "M=D" << endl;
	}

	if (command == "gt") {
		fout << "D=M-D" << endl;
		
		fout << "@gtTrue" << labelCount << endl;
		fout << "D;JGT" << endl;
		fout << "D=0" << endl;
		fout << "@gtDone" << labelCount << endl;
		fout << "0;JMP" << endl;

		fout << "(gtTrue" << labelCount << ")" << endl;
		fout << "D=-1" << labelCount << endl;

		fout << "(gtDone" << labelCount << ")" << endl;
		fout << "@SP" << endl;
		fout << "A=M-1" << endl;
		fout << "A=A-1" << endl;
		fout << "M=D" << endl;
	}

	if (command == "eq") {
		fout << "D=M-D" << endl;

		fout << "@eqTrue" << labelCount << endl;
		fout << "D;JEQ" << endl;
		fout << "D=0" << endl;
		fout << "@eqDone" << labelCount << endl;
		fout << "0;JMP" << endl;

		fout << "(eqTrue" << labelCount << ")" << endl;
		fout << "D=-1" << labelCount << endl;

		fout << "(eqDone" << labelCount << ")" << endl;
		fout << "@SP" << endl;
		fout << "A=M-1" << endl;
		fout << "A=A-1" << endl;
		fout << "M=D" << endl;
	}
	
	fout << "@SP" << endl;
	fout << "M=M-1" << endl;

	labelCount++;
}

string Writer::formatSegment(string segment) {
	if (segment == "local") return "LCL";
	if (segment == "argument") return "ARG";
	if (segment == "this") return "THIS";
	if (segment == "that") return "THAT";

	return segment;
}

void Writer::writePush(string segment, int index) {
	if (segment == "constant") {
		fout << "@" << index << endl;
		fout << "D=A" << endl;
	} else if (segment == "temp") {
		fout << "@R5" << endl;
		fout << "D=A" << endl;
		fout << "@" << index << endl;
		fout << "A=D+A" << endl;
		fout << "D=M" << endl;
	} else if (segment == "pointer") {
		if (index == 0) fout << "@THIS" << endl;
		else fout << "@THAT" << endl;

		fout << "D=M" << endl;
	} else {
		fout << "@" << segment << endl;
		fout << "D=M" << endl;
		fout << "@" << index << endl;
		fout << "A=D+A" << endl;
		fout << "D=M" << endl;
	}

	fout << "@SP" << endl;
	fout << "A=M" << endl;
	fout << "M=D" << endl;
	fout << "@SP" << endl;
	fout << "M=M+1" << endl;
}

void Writer::writePop(string segment, int index) {
	fout << "@SP" << endl;
	fout << "A=M-1" << endl;
	fout << "D=M" << endl;
	fout << "@R13" << endl;
	fout << "M=D" << endl;

	if (segment == "temp") {
		fout << "@R5" << endl;
		fout << "D=A" << endl;
		fout << "@" << index << endl;
		fout << "D=D+M";
	}
	else if (segment == "pointer") {
		if (index == 0) fout << "@THIS" << endl;
		else fout << "@THAT" << endl;

		fout << "D=A" << endl;
	} else {
		fout << "@" << segment << endl;
		fout << "D=M" << endl;
		fout << "@" << index << endl;
		fout << "D=D+A" << endl;
	}
	fout << "@R14" << endl;
	fout << "M=D" << endl;

	fout << "@R13" << endl;
	fout << "D=M" << endl;
	fout << "@R14" << endl;
	fout << "A=M" << endl;
	fout << "M=D" << endl;

	fout << "@SP" << endl;
	fout << "M=M-1" << endl;
}

void Writer::writePushPop(const string command, string segment, int index) {
	string fSeg = formatSegment(segment);
	
	if (command == "C_POP") writePop(fSeg, index);
	if (command == "C_PUSH") writePush(fSeg, index);
}

void Writer::writeLabel(string label) {
	fout << "(";
	fout << label << ")" << endl;
}

void Writer::writeInit() {
	fout << "@256" << endl;
	fout << "D=A" << endl;
	fout << "@SP" << endl;
	fout << "M=D" << endl;

	writeCall("sys.init", 0);
}

void Writer::writeGoTo(string dest) {
	fout << "@" << dest << endl;
	fout << "0;JMP" << endl;
}

void Writer::writeIf(string dest) {
	fout << "@SP" << endl;
	fout << "M=M-1" << endl;
	fout << "A=M" << endl;
	fout << "D=M" << endl;
	fout << "@" << dest << endl;
	fout << "D;JNE" << endl;
}

void Writer::writePushR(string origin) {
	fout << "@" << origin << endl;
	fout << "D=M" << endl;
	fout << "@SP" << endl;
	fout << "A=M" << endl;
	fout << "M=D" << endl;
	fout << "@SP" << endl;
	fout << "M=M+1" << endl;
}

void Writer::writePopR(string origin) {
	fout << "@SP" << endl;
	fout << "A=A-1" << endl;
	fout << "D=M" << endl;
	fout << "@" << origin << endl;
	fout << "M=D" << endl;
	fout << "@SP" << endl;
	fout << "M=M-1" << endl;
}

void Writer::writeCall(string functionName, int args) {
	unsigned int labelID = labelCount++;
	string returnAddressLabel("retPoint");
	returnAddressLabel.append(functionName);
	returnAddressLabel.append(to_string(labelID));

	fout << "@" << returnAddressLabel << endl;
	fout << "D=A" << endl;
	fout << "@SP" << endl;
	fout << "A=M" << endl;
	fout << "M=D" << endl;
	fout << "@SP" << endl;
	fout << "M=M+1" << endl;

	writePushR("LCL");
	writePushR("ARG");
	writePushR("THIS");
	writePushR("THAT");

	//Setting arg pointer to start of function call
	fout << "@SP" << endl;
	fout << "D=M" << endl;
	fout << "@" << args << endl;
	fout << "D=D-A" << endl;
	fout << "@5" << endl;
	fout << "D=D-A" << endl;
	fout << "@ARG" << endl;
	fout << "M=D" << endl;
	//Setting stack pointer to equal local
	fout << "@SP" << endl;
	fout << "D=M" << endl;
	fout << "@LCL" << endl;
	fout << "M=D" << endl;

	writeGoTo(functionName);
	writeLabel(returnAddressLabel);
}

void Writer::writeReturn() {
	fout << "@LCL" << endl;
	fout << "D=M" << endl;
	fout << "@R15" << endl;
	fout << "M=D" << endl;
	
	fout << "@5" << endl;
	fout << "A=D-A" << endl;
	fout << "D=M" << endl;
	fout << "@R16" << endl;
	fout << "M=D" << endl;

	writePop("ARG", 0);

	fout << "@ARG" << endl;
	fout << "D=M+1" << endl;
	fout << "@SP" << endl;
	fout << "M=D" << endl;

	//Restoring caller stack
	const string reg[4] = { "THAT", "THIS", "ARG", "LCL" };
	for (unsigned int i = 0; i < 4; i++) {
		fout << "@R15" << endl;
		fout << "D=M" << endl;
		fout << "A=D-1" << endl;
		fout << "D=M" << endl;
		fout << "@" << reg[i] << endl;
		fout << "M=D" << endl;
		fout << "@R15" << endl;
		fout << "M=M-1" << endl;
	}

	fout << "@R16" << endl;
	fout << "A=M" << endl;
	fout << "0;JMP" << endl;
}

void Writer::writeFunction(string functionName, int args) {
	writeLabel(functionName);
	while (args > 0) {
		writePush("constant", 0);
		args--;
	}
}

void Writer::writeBranching(string command, string name, int numArgs) {
	if (command == "goto") {
		writeGoTo(name); 
	}
	if (command == "if-goto") {
		writeIf(name); 
	}
	if (command == "label") {
		writeLabel(name);
	}
	if (command == "call") {
		writeCall(name, numArgs); 
	}
	if (command == "function") {
		writeFunction(name, numArgs); 
	}
	if (command == "return") {
		writeReturn(); 
	}
	
	return;
}

void Writer::close() {

}
