#pragma once

// #include <iostream> // Really?
#include <sstream> // really?
#include <fstream>
#include <string>
#include "parser.hpp"

class Writer
{
private:
	std::ofstream &file; // Have this be hidden (writeLine() only way to write)
	std::string mFilename;
	int counter = 0;
    int line_num = 0;
    inline const static std::map<std::string, std::string> segmentToSymMap = {
        {"local", "LCL"},
        {"argument", "ARG"},
        {"this", "THIS"},
        {"that", "THAT"}};

    // TODO: Move to .cpp
    int retrieveCounter() {
        counter++;
        return counter++;
    }

    // TODO: Move to .cpp
    template<typename... Args>
    void writeLine(std::string arg, Args... args) {
        // Write args as-is
        file << arg;
        ((file << args), ...);
        if ((arg.rfind("//", 0) != 0) && // Not comment
            (arg.rfind("  //", 0) != 0) && // Not indented comment
            (arg.rfind("(", 0) != 0)) { // Not label
            int offset = 48 - arg.length();
            if (offset > 0)
                file << std::string(offset, ' ');
            // TODO: Fix offset
            file << "// LINE_NUMBER: " << line_num;
            line_num++;
        }

        file << std::endl;
    }
public:
    Writer(std::ofstream &f);
    void setFilename(std::string filename);
	void writeArithemtic(std::string op);
	void writePushPop(CommandType type, std::string seg, int index);
	// TODO: Most of these don't need CommandType args
	void writeInit(CommandType type, std::string seg, int index);
	void writeLabel(CommandType type, std::string label);
	void writeGoto(CommandType type, std::string label);
	void writeIf(CommandType type, std::string label);
	void writeCall(std::string functionName, int numArgs);
	void writeReturn(std::string seg, int index);
	void writeFunction(std::string functionName, int numLocals);

    // TODO: Move to .cpp
    void writeBootstrap() {
        // Store 256 in D register
        writeLine("@256");
        writeLine("D=A");
        // Update SP location to 256
        writeLine("@SP");
        writeLine("M=D");
        // Calls arg-less "Sys.init" function
        writeCall("Sys.init", 0);
        writeLine("@END");
        writeLine("0;JMP");
    }
};
