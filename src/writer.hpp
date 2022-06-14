#ifndef WRITER_H_INCLUDED
#define WRITER_H_INCLUDED

// #include <iostream> // Really?
#include <sstream> // really?
#include <fstream>
#include <string>
#include "command_type.hpp"

class Writer
{
private:
	std::ofstream &file; // Have this be hidden (writeLine() only way to write)
	std::string mFilename;
	int counter = 0;
    int line_num = 0;
    std::map<std::string, std::string> segMap;	// Make this static

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
        ((file << " " << args), ...);
        // If first arg is not comment, write line number
        if ((arg.rfind("//", 0) != 0) ||
            (arg.rfind("  //", 0) != 0)) {
            file << " // LINE_NUMBER: " << line_num;
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

    // TODO: SHOULD IT SAVE EMPTY MEMORY SEGMENTS OR JUMP IMMEDIATELY? WHAT DOES SYS.INIT DO????!!!
    // TODO: Move to .cpp
    // Bootstraps VM implementation....This causes failures?
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

#endif
