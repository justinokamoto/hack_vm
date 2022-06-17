#pragma once

#include <string>
#include <fstream>
    
enum CommandType
{
	C_ARITHMETIC,
	C_PUSH,
	C_POP,
	C_LABEL,
	C_GOTO,
	C_IF,
	C_FUNCTION,
	C_RETURN,
	C_CALL
};

class Parser
{
private:
    std::ifstream &file;
    std::string mCommand;
    std::string mArg1;
	int mArg2;

public:
	Parser(std::ifstream &f);
	bool hasMoreCommands();
	void advance();
	CommandType commandType();
    std::string command();
    std::string arg1();
	int arg2();
};
