// TODO: Try step debugging to see how to do this for larger programs

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>

using namespace std;

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

bool isspace(string input) {
    char c;
    int i = 0;
    while (input[i])
    {
        c = input[i];
	if (!isspace(c)) return false;
    }
    return true;
}

class Writer {
private:
    ofstream& file;
public:
    Writer(ofstream& f) : file(f) {}
    void setFilename(string);
    void writeArithemtic(string op) {
	file << op << endl;
    }
    void writePushPop(CommandType type, string seg, int index) {
	if (type == C_PUSH) {
	    file << "pop " << seg << " " << index << endl;
	} else if (type == C_POP) {
	    file << "pop " << seg << " " << index << endl;
	} else {
	    cout << "Only push/pop commands allowed here.";
	    // TODO: Exception
	}
    }
};

class Parser {
private:
    ifstream& file;
    string mCommand;
    string mArg1;
    int mArg2;
public:
    Parser(ifstream& f) : file(f) {}
    bool hasMoreCommands() {
        if (file.eof()) {
            return false;
        }
        return true;
    }
    void advance() {
        string l;
        if (getline(file, l)) {
            istringstream iss(l);
            int i = 0;
	    vector<string> words;
	    for (string l; iss >> l; ) {
		words.push_back(l);
	    }
	    if (words.size() == 0) // empty line
		return advance();
	    else if (words[0].compare("//") == 0) // comment
		return advance();
	    else if (words.size() > 3)
		cout << "Cannot have " << words.size() << "arguments." << endl;
	    mCommand = words[0];
	    mArg1 = words[1];
	    mArg2 = (words.size() == 3) ? stoi(words[2]) : 0;
        }
    }
    CommandType commandType() {
	CommandType c;
	if (mCommand.compare("push") == 0) {
	    c = C_PUSH;
	} else if (mCommand.compare("pop") == 0) {
	    c = C_POP;
	} else if (mCommand.compare("add") == 0 ||
		   mCommand.compare("sub") == 0 ||
		   mCommand.compare("lt") == 0 ||
		   mCommand.compare("gt") == 0 ||
		   mCommand.compare("eq") == 0 ||
		   mCommand.compare("and") == 0 ||
		   mCommand.compare("or") == 0 ||
		   mCommand.compare("neg") == 0 ||
		   mCommand.compare("not") == 0) {
	    c = C_ARITHMETIC;
	} else {
	    c = C_GOTO;
	    cout << "Unknown command type: " << mCommand << endl;
	    // TODO: Exception
	}
        return c;
    }
    string command()
    {
	return mCommand;
    }
    string arg1()
    {
        return mArg1;
    }
    int arg2()
    {
        return mArg2;
    }
};


int main(int argc, char** argv)
{
    if (argc != 3) {
        cout << "Only 2 arguments allowed." << endl;
        return 1;
    }
    ifstream inputfile (argv[1]);
    ofstream outfile (argv[2]);
    if (inputfile.is_open() && outfile.is_open()) {
        Parser parser = Parser(inputfile);
	Writer writer = Writer(outfile);
        while (true){
            parser.advance();
	    parser.commandType();
	    if (!parser.hasMoreCommands()) {
		break;
	    }
            if (parser.commandType() == C_ARITHMETIC) {
                writer.writeArithemtic(parser.command());
            } else if (parser.commandType() == C_PUSH ||
                       parser.commandType() == C_POP) {
                writer.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
            } else {
                // TODO: Exception
                cout << "Unsupported command type." << endl;
            }
        }
        inputfile.close();
    } else {
        cout << "Couldn't open in/out files." << endl;
        return 1;
    }
    return 0;
}

