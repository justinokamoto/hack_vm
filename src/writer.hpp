#pragma once

#include "parser.hpp" // TODO: Pull out command type to prevent including full parser.hpp?

#include <map>
#include <string>

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

    int retrieveCounter();

    template<typename... Args>
    void writeLine(std::string arg, Args... args);

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
    void writeBootstrap();
};
