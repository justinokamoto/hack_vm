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
	std::ofstream &file;
	std::string mFilename;
	int counter;
    std::map<std::string, std::string> segMap;	// Make this static
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
};

#endif
