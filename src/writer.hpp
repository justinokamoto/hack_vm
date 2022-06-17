#pragma once

#include "parser.hpp" // TODO: Pull out command type to prevent including full parser.hpp?

#include <map>
#include <string>

class Writer {
private:
    std::ofstream& file; // Have this be hidden (writeLine() only way to write)
    std::string filename;
    int branch_num = 0;
    int line_num = 0;
    inline const static std::map<std::string, std::string> segmentToSymMap = {
        { "local", "LCL" },
        { "argument", "ARG" },
        { "this", "THIS" },
        { "that", "THAT" }
    };

    int retrieveCounter();

    template <typename... Args>
    void writeLine(std::string arg, Args... args);

public:
    Writer(std::ofstream& f);
    void setFilename(const std::string& fn);
    void write(CommandType type, const std::string& cmd, const std::string& arg1, int arg2);

    void writeArithemtic(const std::string& op);
    void writePushPop(CommandType type, const std::string& seg, int index);
    // TODO: Most of these don't need CommandType args
    void writeLabel(CommandType type, const std::string& label);
    void writeGoto(CommandType type, const std::string& label);
    void writeIf(CommandType type, const std::string& label);
    void writeCall(const std::string& functionName, int numArgs);
    void writeReturn(const std::string& seg, int index);
    void writeFunction(const std::string& functionName, int numLocals);
    void writeBootstrap();
};
