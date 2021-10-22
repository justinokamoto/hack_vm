// See what we can remove here (copied from main.cpp)
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "writer.hpp"

using namespace std;

Writer::Writer(ofstream &f): file(f)
{
    segMap.insert(pair<string, string>("local", "LCL"));
    segMap.insert(pair<string, string>("argument", "ARG"));
    segMap.insert(pair<string, string>("this", "THIS"));
    segMap.insert(pair<string, string>("that", "THAT"));
    counter = 0;
}

void Writer::setFilename(string filename)
{
    mFilename = filename;
}

void Writer::writeArithemtic(string op)
{
    counter++; // Only do when needed!
    file << "// " << op << endl;
    if (op.compare("add") == 0)
        {
            file << "@SP" << endl;
            file << "A=M-1" << endl;
            file << "D=M" << endl;
            file << "A=A-1" << endl;
            file << "M=M+D" << endl;
            file << "@SP" << endl;
            file << "M=M-1" << endl;
        }
    else if (op.compare("sub") == 0)
        {
            file << "@SP" << endl;
            file << "A=M-1" << endl;
            file << "D=M" << endl;
            file << "A=A-1" << endl;
            file << "M=M-D" << endl;
            file << "@SP" << endl;
            file << "M=M-1" << endl;
        }
    else if (op.compare("lt") == 0)
        {
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
        }
    else if (op.compare("gt") == 0)
        {
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
        }
    else if (op.compare("eq") == 0)
        {
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
        }
    else if (op.compare("and") == 0)
        {
            file << "@SP" << endl;
            file << "A=M-1" << endl;
            file << "D=M" << endl;
            file << "A=A-1" << endl;
            file << "M=D&M" << endl;
            file << "@SP" << endl;
            file << "M=M-1" << endl;
        }
    else if (op.compare("or") == 0)
        {
            file << "@SP" << endl;
            file << "A=M-1" << endl;
            file << "D=M" << endl;
            file << "A=A-1" << endl;
            file << "M=D|M" << endl;
            file << "@SP" << endl;
            file << "M=M-1" << endl;
        }
    else if (op.compare("neg") == 0)
        {
            file << "@SP" << endl;
            file << "A=M-1" << endl;
            file << "M=-M" << endl;
        }
    else if (op.compare("not") == 0)
        {
            file << "@SP" << endl;
            file << "A=M-1" << endl;
            file << "M=!M" << endl;
        }
    else
        {
            cout << "Mismatched arithmetic operator " << op;
            // TODO: Throw exception
        }
}
void Writer::writePushPop(CommandType type, string seg, int index)
{
    counter++;
    if (type == C_PUSH)
        {
            file << "// push " << seg << " " << index << endl;
            if (segMap.find(seg) != segMap.end())
                {
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
                }
            else if (seg.compare("constant") == 0)
                {
                    file << "@" << index << endl;
                    file << "D=A" << endl;
                    file << "@SP" << endl;
                    file << "A=M" << endl;
                    file << "M=D" << endl;
                    file << "@SP" << endl;
                    file << "M=M+1" << endl;
                }
            else if (seg.compare("temp") == 0)
                {
                    file << "@5" << endl;
                    file << "D=A" << endl;
                    file << "@" << index << endl;
                    file << "A=A+D" << endl;
                    file << "D=M" << endl;
                    file << "@SP" << endl;
                    file << "M=M+1" << endl;
                    file << "A=M-1" << endl;
                    file << "M=D" << endl;
                }
            else if (seg.compare("pointer") == 0)
                {
                    string seg;
                    if (index == 0)
                        {
                            seg = "THIS";
                        }
                    else if (index == 1)
                        {
                            seg = "THAT";
                        }
                    else
                        {
                            cout << "Invalid push to pointer " << index << endl;
                            // TODO: Raise exception
                        }
                    file << "@" << seg << endl;
                    file << "D=M" << endl;
                    file << "@SP" << endl;
                    file << "M=M+1" << endl;
                    file << "A=M-1" << endl;
                    file << "M=D" << endl;
                }
            else if (seg.compare("static") == 0)
                {
                    file << "@" << mFilename << "." << index << endl;
                    file << "D=M" << endl;
                    file << "@SP" << endl;
                    file << "M=M+1" << endl;
                    file << "A=M-1" << endl;
                    file << "M=D" << endl;
                }
            else
                {
                    cout << "Invalid segment pop " << seg << endl;
                }
        }
    else if (type == C_POP)
        {
            file << "// pop " << seg << " " << index << endl;
            if (segMap.find(seg) != segMap.end())
                {
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
                }
            else if (seg.compare("temp") == 0)
                {
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
                }
            else if (seg.compare("pointer") == 0)
                {
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
                }
                else if (seg.compare("static") == 0)
                {
                        file << "@SP" << endl;
                        file << "M=M-1" << endl;
                        file << "A=M" << endl;
                        file << "D=M" << endl;
                        file << "@" << mFilename << "." << index << endl;
                        file << "M=D" << endl;
                }
                else
                {
                        cout << "Invalid segment pop " << seg << endl;
                }
        }
        else
        {
                cout << "Only push/pop commands allowed here.";
                // TODO: Exception
        }
}
// TODO: Most of these don't need CommandType args
void Writer::writeInit(CommandType type, string seg, int index)
{
        file << "// " << endl;
}
void Writer::writeLabel(CommandType type, string label)
{
        file << "// label " << label << endl;
        file << "(" << label << ")" << endl;
}
void Writer::writeGoto(CommandType type, string label)
{
        file << "// goto " << label << endl;
        file << "@" << label << endl;
        file << "0;JMP" << endl;
}
void Writer::writeIf(CommandType type, string label)
{
        file << "// if-goto " << label << endl;
        file << "@SP" << endl; // Set A to SP
        file << "M=M-1" << endl; // Get recent SP mem address
        file << "A=M" << endl; // Set memory to recent SP mem address
        file << "D=M" << endl; // Set D register to M
        file << "@" << label << endl;
        file << "D;JGT" << endl;
}
void Writer::writeCall(CommandType type, string seg, int index)
{
        file << "// " << endl;
}
void Writer::writeReturn(CommandType type, string seg, int index)
{
        file << "// " << endl;
}
void Writer::writeFunction(string functionName, int numArgs)
{
        file << "// function " << functionName << " " << numArgs << endl;
        // TODO: Add function to map -> line and check if exists already?
        // Create label
        file << "@" << functionName << endl;
        // push to argument mem stack?
}

