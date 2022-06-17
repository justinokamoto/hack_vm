#include "parser.hpp"
#include <iostream>
#include <sstream>
#include <vector>

using namespace std;

Parser::Parser(std::ifstream &f) : file(f) {}

bool Parser::hasMoreCommands()
{
    if (file.eof())
		{
			return false;
		}
    return true;
}

void Parser::advance()
{
    string l;
    if (getline(file, l)) {
        istringstream iss(l);
        int i = 0;
        vector<string> words;
        for (string l; iss >> l;) {
            words.push_back(l);
        }
        // Validations
        if (words.size() == 0) // empty line
            return advance();
        else if (words[0].compare("//") == 0) // comment
            return advance();
        else if (words.size() > 3) {
            // If 4 words exist (all non-comments) that's an error
            if (words[1].compare("//") != 0 &&
                words[2].compare("//") != 0 &&
                words[3].compare("//") != 0)
                cout << "Line \"" << l << "\" has too many args." << endl;
        }
        // Command is valid, now parse our args
        mCommand = words[0];
        mArg1 = "";
        mArg2 = 0;

        // If single word, return
        if (words.size() == 1)
            return;
        // If comment after single word, return
        if (words[1].compare("//") == 0)
            return;

        mArg1 = words[1];
        // If two words, return
        if (words.size() == 2)
            return;
        // If comment after two words, return
        if (words[2].compare("//") == 0)
            return;

        mArg2 = stoi(words[2]);
    }
}

CommandType Parser::commandType()
{
    CommandType c;
    if (mCommand.compare("push") == 0) {
        c = C_PUSH;
    }
    else if (mCommand.compare("pop") == 0) {
        c = C_POP;
    }
    else if (mCommand.compare("add") == 0 ||
             mCommand.compare("sub") == 0 ||
             mCommand.compare("lt") == 0 ||
             mCommand.compare("gt") == 0 ||
             mCommand.compare("eq") == 0 ||
             mCommand.compare("and") == 0 ||
             mCommand.compare("or") == 0 ||
             mCommand.compare("neg") == 0 ||
             mCommand.compare("not") == 0) {
        c = C_ARITHMETIC;
    }
    else if (mCommand.compare("function") == 0) {
        c = C_FUNCTION;
    }
    else if (mCommand.compare("return") == 0) {
        c = C_RETURN;
    }
    else if (mCommand.compare("call") == 0) {
        c = C_CALL;
    }
    else if (mCommand.compare("label") == 0) {
        c = C_LABEL;
    }
    else if (mCommand.compare("if-goto") == 0) {
        c = C_IF;
    }
    else if (mCommand.compare("goto") == 0) {
        c = C_GOTO;
    }
    else if (mCommand.compare("function") == 0) {
        c = C_FUNCTION;
    }
    else {
        c = C_GOTO;
        cout << "Unknown command type: " << mCommand << endl;
        // TODO: Exception
    }
    return c;
}

string Parser::command() { return mCommand; }

string Parser::arg1() { return mArg1; }

int Parser::arg2() { return mArg2; }
