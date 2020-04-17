// TODO: Try step debugging to see how to do this for larger programs

#include <iostream>
#include <fstream>
#include <map>
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

map<string, string> segMap;

map<string, string>& createSegMap() {
    if (segMap.empty()) {
	segMap.insert(pair<string,string>("local", "LCL"));
	segMap.insert(pair<string,string>("argument", "ARG"));
	segMap.insert(pair<string,string>("this", "THIS"));
	segMap.insert(pair<string,string>("that", "THAT"));
    }
    return segMap;
}

class Writer {
private:
    ofstream& file;
    string mFilename;
    int counter;
public:
    Writer(ofstream& f) : file(f) {
	counter = 0;
    }
    void setFilename(string filename) {
	mFilename = filename;
    }
    void writeArithemtic(string op) {
	counter++; // Only do when needed!
	file << "// " << op << endl;
	if (op.compare("add") == 0) {
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "D=M" << endl;
	    file << "A=A-1" << endl;
	    file << "M=M+D" << endl;
	    file << "@SP" << endl;
	    file << "M=M-1" << endl;
	} else if (op.compare("sub") == 0) {
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "D=M" << endl;
	    file << "A=A-1" << endl;
	    file << "M=M-D" << endl;
	    file << "@SP" << endl;
	    file << "M=M-1" << endl;
	} else if (op.compare("lt") == 0) {
	    file << "@SP" << endl;
	    file << "M=M-1" << endl;
	    file << "A=M" << endl;
	    file << "D=M" << endl;
	    file << "A=A-1" << endl;
	    file << "D=M-D" << endl;
	    file << "@BRANCH_TRUE_" << counter << endl;
	    file << "D;JLT" << endl;
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=0" << endl;
	    file << "@END_BRANCH_" << counter << endl;
	    file << "0;JMP" << endl;
	    file << "(BRANCH_TRUE_" << counter << ")" << endl;
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=-1" << endl;
	    file << "(END_BRANCH_" << counter << ")" << endl;
	} else if (op.compare("gt") == 0) {
	    file << "@SP" << endl;
	    file << "M=M-1" << endl;
	    file << "A=M" << endl;
	    file << "D=M" << endl;
	    file << "A=A-1" << endl;
	    file << "D=M-D" << endl;
	    file << "@BRANCH_TRUE_" << counter << endl;
	    file << "D;JGT" << endl;
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=0" << endl;
	    file << "@END_BRANCH_" << counter << endl;
	    file << "0;JMP" << endl;
	    file << "(BRANCH_TRUE_" << counter << ")" << endl;
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=-1" << endl;
	    file << "(END_BRANCH_" << counter << ")" << endl;
	} else if (op.compare("eq") == 0) {
	    file << "@SP" << endl;
	    file << "M=M-1" << endl;
	    file << "A=M" << endl;
	    file << "D=M" << endl;
	    file << "A=A-1" << endl;
	    file << "D=D-M" << endl;
	    file << "@BRANCH_TRUE_" << counter << endl;
	    file << "D;JEQ" << endl;
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=0" << endl;
	    file << "@END_BRANCH_" << counter << endl;
	    file << "0;JMP" << endl;
	    file << "(BRANCH_TRUE_" << counter << ")" << endl;
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=-1" << endl;
	    file << "(END_BRANCH_" << counter << ")" << endl;
	} else if (op.compare("and") == 0) {
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "D=M" << endl;
	    file << "A=A-1" << endl;
	    file << "M=D&M" << endl;
	    file << "@SP" << endl;
	    file << "M=M-1" << endl;
	} else if (op.compare("or") == 0) {
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "D=M" << endl;
	    file << "A=A-1" << endl;
	    file << "M=D|M" << endl;
	    file << "@SP" << endl;
	    file << "M=M-1" << endl;      
	} else if (op.compare("neg") == 0) {
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=-M" << endl;
	} else if (op.compare("not") == 0) {
	    file << "@SP" << endl;
	    file << "A=M-1" << endl;
	    file << "M=!M" << endl;
	} else {
	    cout << "Mismatched arithmetic operator " << op;
	    // TODO: Throw exception
	}
    }
    void writePushPop(CommandType type, string seg, int index) {
	counter++;
	if (type == C_PUSH) {
	    file << "// push " << seg << " " << index << endl;
	    if (segMap.find(seg) != segMap.end()) {
		file << "@" << index << endl;
		file << "D=A" << endl;
		file << "@" << segMap[seg] << endl;
		file << "A=M" << endl;
		file << "A=A+D" << endl;
		file << "D=M" << endl;
		file << "@SP" << endl;
		file << "M=M+1" << endl;
		file << "A=M-1" << endl;
		file << "M=D" << endl;
	    } else if (seg.compare("constant") == 0) {
		file << "@" << index << endl;
		file << "D=A" << endl;
		file << "@SP" << endl;
		file << "A=M" << endl;
		file << "M=D" << endl;
		file << "@SP" << endl;
		file << "M=M+1" << endl;
	    } else if (seg.compare("temp") == 0) {
		file << "@5" << endl;
		file << "D=A" << endl;
		file << "@" << index << endl;
		file << "A=A+D" << endl;
		file << "D=M" << endl;
		file << "@SP" << endl;
		file << "M=M+1" << endl;
		file << "A=M-1" << endl;
		file << "M=D" << endl;	
	    } else if (seg.compare("pointer") == 0) {
		string seg;
		if (index == 0) {
		    seg = "THIS";
		} else if (index == 1) {
		    seg = "THAT";
		} else {
		    cout << "Invalid push to pointer " << index << endl;
		    // TODO: Raise exception
		}
		file << "@" << seg << endl;
		file << "D=M" << endl;
		file << "@SP" << endl;
		file << "M=M+1" << endl;
		file << "A=M-1" << endl;
		file << "M=D" << endl;
	    } else if (seg.compare("static") == 0) {
		file << "@" << mFilename << "." << index << endl;
		file << "D=M" << endl;
		file << "@SP" << endl;
		file << "M=M+1" << endl;
		file << "A=M-1" << endl;
		file << "M=D" << endl;
	    } else {
		cout << "Invalid segment pop " << seg << endl;
	    }
	}
	else if (type == C_POP) {
	    file << "// pop " << seg << " " << index << endl;
	    if (segMap.find(seg) != segMap.end()) {
		file << "@" << index << endl;
		file << "D=A" << endl;
		file << "@" << segMap[seg] << endl;
		file << "A=M" << endl;
		file << "D=A+D" << endl;
		file << "@R13" << endl;
		file << "M=D" << endl;
		file << "@SP" << endl;
		file << "M=M-1" << endl;
		file << "A=M" << endl;
		file << "D=M" << endl;
		file << "@R13" << endl;
		file << "A=M" << endl;
		file << "M=D" << endl;
	    } else if (seg.compare("temp") == 0) {
		file << "@5" << endl;
		file << "D=A" << endl;
		file << "@" << index << endl;
		file << "D=A+D" << endl;
		file << "@R13" << endl;
		file << "M=D" << endl;
		file << "@SP" << endl;
		file << "M=M-1" << endl;
		file << "A=M" << endl;
		file << "D=M" << endl;
		file << "@R13" << endl;
		file << "A=M" << endl;
		file << "M=D" << endl;
	    } else if (seg.compare("pointer") == 0) {
		string segToken = (index == 0) ? "THIS" : "THAT";
		file << "@" << segToken << endl;
		file << "D=A" << endl;
		file << "@R13" << endl;
		file << "M=D" << endl;
		file << "@SP" << endl;
		file << "M=M-1" << endl;
		file << "A=M" << endl;
		file << "D=M" << endl;
		file << "@R13" << endl;
		file << "A=M" << endl;
		file << "M=D" << endl;
	    } else if (seg.compare("static") == 0) {
		file << "@SP" << endl;
		file << "M=M-1" << endl;
		file << "A=M" << endl;
		file << "D=M" << endl;
		file << "@" << mFilename << "." << index << endl;
		file << "M=D" << endl;
	    } else {
		cout << "Invalid segment pop " << seg << endl;
	    }
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
	createSegMap(); // initialize segmap
        Parser parser = Parser(inputfile);
	Writer writer = Writer(outfile);

	string filename;
	size_t found = string(argv[2]).find('.');
	if (found != string::npos) {
	    filename.append(argv[2], found);
	} else {
	    filename.append(argv[2]);
	}
	writer.setFilename(filename);
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
	outfile.close();
    } else {
        cout << "Couldn't open in/out files." << endl;
        return 1;
    }
    return 0;
}

