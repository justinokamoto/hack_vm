#pragma once

#include <fstream>
#include <string>

enum CommandType {
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

class Parser {
private:
    std::ifstream& file; // TODO: Should tie lifecycle of stream to parser!
    std::string mCommand;
    std::string mArg1;
    int mArg2;

public:
    Parser(std::ifstream& f);
    bool hasMoreCommands();
    void advance();
    // All getters are copies as data (string included) are guaranteed
    // to be small in this context and we can avoid dangling
    // references / managing smart pointers
    CommandType commandType();
    std::string command();
    std::string arg1();
    int arg2();
};
