// See what we can remove here (copied from main.cpp)
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include "writer.hpp"

using namespace std;

// TODO: Convenience functions for...
// - advancing SP
// - pushing to SP
// - writing assembly (writeLine()) or something...

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
                    file << "@5" << endl; // What?
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
// TODO: CREATE moveSP(int num) for convenience here!
void Writer::writeCall(string functionName, int numArgs)
{
    counter++; // Only do when needed!

    // CALLING CONVENTION
    // VM function already accounts for:
    // - pushing number of args onto stack
    // - numArgs is how many args have been pushed

    // VM implementation
    // - push return addr to stack
    // - push LCL addr to stack
    // - push ARG addr to stack
    // - push THIS addr to stack
    // - push THAT addr to stack
    // - Reposition ARG
    // - Reposition LCL
    // - Jump to function
    // NOTE: callee will offset SP to LCL + num locals
    file << "// call " << functionName << " " << numArgs << endl;

    // Push return addr, LCL addr, ARG addr, THIS addr, and THAT addr
    // to stack.
    std::string ret_label = "ret_addr_" + to_string(retrieveCounter());

    file << "//   push @" << ret_label << " to stack " << endl;
    // Store addr in D
    file << "@" << ret_label << endl;
    file << "D=A" << endl;
    // Advance SP
    file << "@SP" << endl;
    file << "M=M+1" << endl;
    // Set prev SP value to D
    file << "A=M-1" << endl;
    file << "M=D" << endl;

    vector<std::string> ret_addrs;
    ret_addrs.push_back("LCL");
    ret_addrs.push_back("ARG");
    ret_addrs.push_back("THIS");
    ret_addrs.push_back("THAT");
    for (std::string addr : ret_addrs) {
        file << "//   push @" << addr << " to stack " << endl;
        // Store addr in D
        file << "@" << addr << endl;
        file << "D=M" << endl;
        // Advance SP
        file << "@SP" << endl;
        file << "M=M+1" << endl;
        // Set prev SP value to D
        file << "A=M-1" << endl;
        file << "M=D" << endl;
    }

    // Reposition LCL
    // Essentially accomplishing LCL = SP
    file << "   // Reposition LCL (LCL=SP)" << endl;
    file << "@SP" << endl;
    file << "D=M" << endl;
    file << "@LCL" << endl;
    file << "M=D" << endl;

    // Reposition ARG
    // ARG = SP - nargs - 5
    file << "   // Reposition ARG (ARG=SP-nargs-5)" << endl;
    // Load SP and store to D
    file << "@SP" << endl;
    file << "D=M" << endl;
    // Load 5 and store to D
    file << "@5" << endl;
    file << "D=D-A" << endl;
    // Load nargs and store to D
    file << "@" << numArgs << endl;
    file << "D=D-A" << endl;
    // Set ARG to D
    file << "@ARG" << endl;
    file << "M=D" << endl;

    // call function
    file << "   // Jump to function address" << endl;
    file << "@" << functionName << endl;
    file << "0;JMP" << endl;
    // Create label to mark current addr in execution
    // Better way to get addr of current mem executing besides label?
    file << "(" << ret_label << ")" << endl; // Make label based on global instruction counter (TODO: Remove this hack)

}
void Writer::writeReturn(string seg, int index)
{
    // TODO: Use segMap instead of creating vetor?
    // TODO: Verify that R13+ are general purpose registers

    // Store the return value at general purpose register R13
    file << "// return" << endl;
    file << "@SP" << endl;
    file << "A=M-1" << endl;
    file << "D=M" << endl;
    file << "@R13" << endl;
    file << "M=D" << endl;

    // Reset SP
    file << "@ARG" << endl; // Set A to ARG ptr
    file << "D=M" << endl;  // Set D to ARG addr
    file << "@SP" << endl;  // Set A to SP ptr
    file << "M=D+1" << endl; // Increment SP ptr to ARG ptr + 1

    // Store LCL in @frame variable
    file << "@LCL" << endl;
    file << "D=M" << endl;
    file << "@frame" << endl;
    file << "M=D" << endl;
    // Walk back and restore all memory segments
    vector<std::string> memSegs;
    memSegs.push_back("THAT");
    memSegs.push_back("THIS");
    memSegs.push_back("ARG");
    memSegs.push_back("LCL");
    for (std::string seg : memSegs) {
        // Walk frame back
        file << "@frame" << endl;
        file << "M=M-1" << endl;
        // Store caller memory segment addr in D
        file << "A=M" << endl;
        file << "D=M" << endl;
        // Set memory segment to D
        file << "@" << seg << endl;
        file << "M=D" << endl;
    }
    // Walk frame back
    file << "@frame" << endl;
    file << "M=M-1" << endl;
    // Load return address and jump to it
    file << "A=M" << endl;
    file << "0;JMP" << endl;
}

// TODO: Add function to map -> line and check if exists already?
void Writer::writeFunction(string functionName, int numLocals)
{
    counter++;
    file << "// function " << functionName << " " << numLocals << endl;
    // Create label
    file << "(" << functionName << ")" << endl;
    // Store numArgs in D
    file << "  // Store numLocals in D" << endl;
    file << "@" << numLocals << endl;
    file << "D=A" << endl;

    // Advance SP (caller keeps SP and LCL at the same position)
    // SP = SP + numLocals
    // Initializes local variables to 0 as SP advances
    file << "  // Init LCL segment values to 0" << endl;
    file << "@BRANCH_INIT_LOCAL_" << functionName << endl;
    file << "D;JGT" << endl;
    file << "@END_INIT_LOCAL_" << functionName << endl;
    file << "0;JMP" << endl;
    file << "(BRANCH_INIT_LOCAL_" << functionName << ")" << endl;
    file << "@SP" << endl;
    file << "M=M+1" << endl; // Advance SP addr
    file << "A=M-1" << endl; // Go to prev SP addr value
    file << "M=0" << endl;   // Init the memory to 0
    file << "D=D-1" << endl; // Decrement numArgs
    file << "@BRANCH_INIT_LOCAL_" << functionName << endl;
    file << "D;JGT" << endl;
    file << "(END_INIT_LOCAL_" << functionName << ")" << endl;
}

