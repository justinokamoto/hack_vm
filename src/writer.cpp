#include "writer.hpp"

#include <fstream>
#include <iostream>
#include <vector>

using namespace std;

// TODO: Convenience functions for...
// - advancing SP
// - pushing to SP
// - writing assembly (writeLine()) or something...

Writer::Writer(ofstream& f)
    : file(f)
{
}

// TODO: Make this hidden (no access to branch_num)
int Writer::retrieveCounter()
{
    branch_num++;
    return branch_num++;
}

template <typename... Args>
void Writer::writeLine(std::string arg, Args... args)
{
    // Write args as-is
    file << arg;
    ((file << args), ...);
    if ((arg.rfind("//", 0) != 0) && // Not comment
        (arg.rfind("  //", 0) != 0) && // Not indented comment
        (arg.rfind("(", 0) != 0)) { // Not label
        int offset = 48 - arg.length();
        if (offset > 0)
            file << std::string(offset, ' ');
        // TODO: Fix offset
        file << "// LINE_NUMBER: " << line_num;
        line_num++;
    }

    file << std::endl;
}

void Writer::write(CommandType type,
    const std::string& cmd,
    const std::string& arg1,
    int arg2)
{
    if (type == C_ARITHMETIC) {
        writeArithemtic(cmd);
    } else if (type == C_PUSH || type == C_POP) {
        writePushPop(type, arg1, arg2);
    } else if (type == C_LABEL) {
        writeLabel(type, arg1);
    } else if (type == C_IF) {
        writeIf(type, arg1);
    } else if (type == C_GOTO) {
        writeGoto(type, arg1);
    } else if (type == C_RETURN) {
        writeReturn(arg1, arg2);
    } else if (type == C_FUNCTION) {
        writeFunction(arg1, arg2);
    } else if (type == C_CALL) {
        writeCall(arg1, arg2);
    } else {
        throw std::invalid_argument("Unsupported command type.");
    }
}

void Writer::setFilename(const string& fn)
{
    filename = fn;
}

void Writer::writeArithemtic(const string& op)
{
    writeLine("// ", op);
    if (op.compare("add") == 0) {
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("D=M");
        writeLine("A=A-1");
        writeLine("M=M+D");
        writeLine("@SP");
        writeLine("M=M-1");
    } else if (op.compare("sub") == 0) {
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("D=M");
        writeLine("A=A-1");
        writeLine("M=M-D");
        writeLine("@SP");
        writeLine("M=M-1");
    } else if (op.compare("lt") == 0) {
        int counter = retrieveCounter();
        writeLine("@SP");
        writeLine("M=M-1");
        writeLine("A=M");
        writeLine("D=M");
        writeLine("A=A-1");
        writeLine("D=M-D");
        writeLine("@BRANCH_TRUE_", counter);
        writeLine("D;JLT");
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=0");
        writeLine("@END_BRANCH_", counter);
        writeLine("0;JMP");
        writeLine("(BRANCH_TRUE_", counter, ")");
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=-1");
        writeLine("(END_BRANCH_", counter, ")");
    } else if (op.compare("gt") == 0) {
        int counter = retrieveCounter();
        writeLine("@SP");
        writeLine("M=M-1");
        writeLine("A=M");
        writeLine("D=M");
        writeLine("A=A-1");
        writeLine("D=M-D");
        writeLine("@BRANCH_TRUE_", counter);
        writeLine("D;JGT");
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=0");
        writeLine("@END_BRANCH_", counter);
        writeLine("0;JMP");
        writeLine("(BRANCH_TRUE_", counter, ")");
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=-1");
        writeLine("(END_BRANCH_", counter, ")");
    } else if (op.compare("eq") == 0) {
        int counter = retrieveCounter();
        writeLine("@SP");
        writeLine("M=M-1");
        writeLine("A=M");
        writeLine("D=M");
        writeLine("A=A-1");
        writeLine("D=D-M");
        writeLine("@BRANCH_TRUE_", counter);
        writeLine("D;JEQ");
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=0");
        writeLine("@END_BRANCH_", counter);
        writeLine("0;JMP");
        writeLine("(BRANCH_TRUE_", counter, ")");
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=-1");
        writeLine("(END_BRANCH_", counter, ")");
    } else if (op.compare("and") == 0) {
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("D=M");
        writeLine("A=A-1");
        writeLine("M=D&M");
        writeLine("@SP");
        writeLine("M=M-1");
    } else if (op.compare("or") == 0) {
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("D=M");
        writeLine("A=A-1");
        writeLine("M=D|M");
        writeLine("@SP");
        writeLine("M=M-1");
    } else if (op.compare("neg") == 0) {
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=-M");
    } else if (op.compare("not") == 0) {
        writeLine("@SP");
        writeLine("A=M-1");
        writeLine("M=!M");
    } else {
        cout << "Mismatched arithmetic operator " << op;
        // TODO: Throw exception
    }
}
void Writer::writePushPop(CommandType type, const string& seg, int index)
{
    if (type == C_PUSH) {
        writeLine("// push ", seg, " ", index);
        if (segmentToSymMap.find(seg) != segmentToSymMap.end()) {
            writeLine("@", index);
            writeLine("D=A");
            writeLine("@", segmentToSymMap.at(seg));
            writeLine("A=M");
            writeLine("A=A+D");
            writeLine("D=M");
            writeLine("@SP");
            writeLine("M=M+1");
            writeLine("A=M-1");
            writeLine("M=D");
        } else if (seg.compare("constant") == 0) {
            writeLine("@", index);
            writeLine("D=A");
            writeLine("@SP");
            writeLine("A=M");
            writeLine("M=D");
            writeLine("@SP");
            writeLine("M=M+1");
        } else if (seg.compare("temp") == 0) {
            // TODO: What?
            writeLine("@5");
            writeLine("D=A");
            writeLine("@", index);
            writeLine("A=A+D");
            writeLine("D=M");
            writeLine("@SP");
            writeLine("M=M+1");
            writeLine("A=M-1");
            writeLine("M=D");
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
            writeLine("@", seg);
            writeLine("D=M");
            writeLine("@SP");
            writeLine("M=M+1");
            writeLine("A=M-1");
            writeLine("M=D");
        } else if (seg.compare("static") == 0) {
            writeLine("@", filename, ".", index);
            writeLine("D=M");
            writeLine("@SP");
            writeLine("M=M+1");
            writeLine("A=M-1");
            writeLine("M=D");
        } else {
            cout << "Invalid segment pop " << seg << endl;
        }
    } else if (type == C_POP) {
        writeLine("// pop ", seg, " ", index);
        if (segmentToSymMap.find(seg) != segmentToSymMap.end()) {
            writeLine("@", index);
            writeLine("D=A");
            writeLine("@", segmentToSymMap.at(seg));
            writeLine("A=M");
            writeLine("D=A+D");
            writeLine("@R13");
            writeLine("M=D");
            writeLine("@SP");
            writeLine("M=M-1");
            writeLine("A=M");
            writeLine("D=M");
            writeLine("@R13");
            writeLine("A=M");
            writeLine("M=D");
        } else if (seg.compare("temp") == 0) {
            writeLine("@5");
            writeLine("D=A");
            writeLine("@", index);
            writeLine("D=A+D");
            writeLine("@R13");
            writeLine("M=D");
            writeLine("@SP");
            writeLine("M=M-1");
            writeLine("A=M");
            writeLine("D=M");
            writeLine("@R13");
            writeLine("A=M");
            writeLine("M=D");
        } else if (seg.compare("pointer") == 0) {
            string segToken = (index == 0) ? "THIS" : "THAT";
            writeLine("@", segToken);
            writeLine("D=A");
            writeLine("@R13");
            writeLine("M=D");
            writeLine("@SP");
            writeLine("M=M-1");
            writeLine("A=M");
            writeLine("D=M");
            writeLine("@R13");
            writeLine("A=M");
            writeLine("M=D");
        } else if (seg.compare("static") == 0) {
            writeLine("@SP");
            writeLine("M=M-1");
            writeLine("A=M");
            writeLine("D=M");
            writeLine("@", filename, ".", index);
            writeLine("M=D");
        } else {
            cout << "Invalid segment pop " << seg << endl;
        }
    } else {
        cout << "Only push/pop commands allowed here.";
        // TODO: Exception
    }
}
// TODO: Most of these don't need CommandType args

void Writer::writeLabel(CommandType type, const string& label)
{
    writeLine("// label ", label);
    writeLine("(", label, ")");
}
void Writer::writeGoto(CommandType type, const string& label)
{
    writeLine("// goto ", label);
    writeLine("@", label);
    writeLine("0;JMP");
}
void Writer::writeIf(CommandType type, const string& label)
{
    writeLine("// if-goto ", label);
    writeLine("@SP");
    writeLine("M=M-1");
    writeLine("A=M");
    writeLine("D=M");
    writeLine("@", label);
    writeLine("D;JNE");
}
// TODO: CREATE moveSP(int num) for convenience here!
void Writer::writeCall(const string& functionName, int numArgs)
{
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
    writeLine("// call ", functionName, " ", numArgs);

    // Push return addr, LCL addr, ARG addr, THIS addr, and THAT addr
    // to stack.
    std::string ret_label = "ret_addr_" + to_string(retrieveCounter());

    writeLine("//   push @", ret_label, " to stack ");
    // Store addr in D
    writeLine("@", ret_label);
    writeLine("D=A");
    // Advance SP
    writeLine("@SP");
    writeLine("M=M+1");
    // Set prev SP value to D
    writeLine("A=M-1");
    writeLine("M=D");

    vector<std::string> ret_addrs;
    ret_addrs.push_back("LCL");
    ret_addrs.push_back("ARG");
    ret_addrs.push_back("THIS");
    ret_addrs.push_back("THAT");
    for (std::string addr : ret_addrs) {
        writeLine("//   push @", addr, " to stack ");
        // Store addr in D
        writeLine("@", addr);
        writeLine("D=M");
        // Advance SP
        writeLine("@SP");
        writeLine("M=M+1");
        // Set prev SP value to D
        writeLine("A=M-1");
        writeLine("M=D");
    }

    // Reposition LCL
    // Essentially accomplishing LCL = SP
    writeLine("  // Reposition LCL (LCL=SP)");
    writeLine("@SP");
    writeLine("D=M");
    writeLine("@LCL");
    writeLine("M=D");

    // Reposition ARG
    // ARG = SP - nargs - 5
    writeLine("  // Reposition ARG (ARG=SP-nargs-5)");
    // Load SP and store to D
    writeLine("@SP");
    writeLine("D=M");
    // Load 5 and store to D
    writeLine("@5");
    writeLine("D=D-A");
    // Load nargs and store to D
    writeLine("@", numArgs);
    writeLine("D=D-A");
    // Set ARG to D
    writeLine("@ARG");
    writeLine("M=D");

    // call function
    writeLine("  // Jump to function address");
    writeLine("@", functionName);
    writeLine("0;JMP");
    // Create label to mark current addr in execution
    // Better way to get addr of current mem executing besides label?
    writeLine("(", ret_label, ")");
}
void Writer::writeReturn(const string& seg, int index)
{
    // TODO: Use segmentToSymMap instead of creating vector?
    // TODO: Verify that R13+ are general purpose registers
    writeLine("// return");

    // Store the return addr at general purpose register R13
    writeLine("  // Store the return addr at general purpose register R13");
    writeLine("@LCL");
    // Store LCL in D
    writeLine("D=M");
    writeLine("@5");
    // Subtract 5 from D
    writeLine("D=D-A");
    // Jump to register storing return addr
    writeLine("A=D");
    // Store return addr
    writeLine("D=M");
    writeLine("@R13");
    // Store return addr in R13
    writeLine("M=D");

    // Set ARG to current SP value
    writeLine("  // Set ARG to return value");
    writeLine("@SP");
    // Jump to previous stack register
    writeLine("A=M-1");
    // Store value in D
    writeLine("D=M");
    writeLine("@ARG");
    writeLine("A=M");
    // Store D at ARG location
    writeLine("M=D");
    // Wind SP back to ARG + 1
    writeLine("  // Restore SP to ARG+1");
    writeLine("@ARG");
    writeLine("D=M");
    writeLine("@SP");
    writeLine("M=D+1");

    // Restore all memory segments
    // in reference to LCL address
    writeLine("  // Restore all memory segments");
    int lcl_offset = 1; // offset from LCL address
    for (std::string seg : vector<std::string> { "THAT", "THIS", "ARG", "LCL" }) {
        writeLine("@", lcl_offset);
        writeLine("D=A");
        writeLine("@LCL");
        writeLine("A=M-D");
        writeLine("D=M");
        writeLine("@", seg);
        writeLine("M=D");
        lcl_offset++;
    }
    writeLine("@R13");
    writeLine("A=M");
    writeLine("0;JMP");
}

// TODO: Add function to map -> line and check if exists already?
void Writer::writeFunction(const string& functionName, int numLocals)
{
    writeLine("// function ", functionName, " ", numLocals);
    // Create label
    writeLine("(", functionName, ")");
    // Store numArgs in D
    writeLine("  // Store numLocals in D");
    writeLine("@", numLocals);
    writeLine("D=A");

    // Advance SP (caller keeps SP and LCL at the same position)
    // SP = SP + numLocals
    // Initializes local variables to 0 as SP advances
    writeLine("  // Init LCL segment values to 0");
    writeLine("@BRANCH_INIT_LOCAL_", functionName);
    writeLine("D;JGT");
    writeLine("@END_INIT_LOCAL_", functionName);
    writeLine("0;JMP");
    writeLine("(BRANCH_INIT_LOCAL_", functionName, ")");
    writeLine("@SP");
    writeLine("M=M+1");
    writeLine("A=M-1");
    writeLine("M=0");
    writeLine("D=D-1");
    writeLine("@BRANCH_INIT_LOCAL_", functionName);
    writeLine("D;JGT");
    writeLine("(END_INIT_LOCAL_", functionName, ")");
}

void Writer::writeBootstrap()
{
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
