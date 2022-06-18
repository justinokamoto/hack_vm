#include "parser.hpp"
#include "writer.hpp"

#include <filesystem>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

// TODO: Better management of file handles
int main(int argc, char** argv)
{
    // TODO: Validate one is Main.vm!
    // TODO: Validate one has main function!

    if (argc != 3) {
        cout << "Only 2 arguments allowed." << endl;
        return 1;
    }

    // TODO: [improvement] make more efficient
    std::vector<filesystem::path> in_filenames;
    if (filesystem::is_directory(argv[1])) {
        for (filesystem::path const& entry : filesystem::directory_iterator(argv[1])) {
            if (entry.extension().compare(".vm") == 0)
                in_filenames.push_back(entry);
        }
    } else {
        in_filenames.push_back(argv[1]);
    }

    ofstream outfile(argv[2]);
    if (!outfile.is_open()) {
        std::cerr << "Could not open output file " << argv[2] << std::endl;
        return 1;
    }

    Writer writer = Writer(outfile); // Define here so `counter` isn't reset (TODO: global counter)
    // Conditionally bootstrap? (add option for this)

    // Debug option for test purposes (early tests don't assume VM is properly bootstrapped)
    if (getenv("DISABLE_BOOTSTRAP")) {
        cerr << "WARNING: VM bootstrapping disabled." << endl;
    } else {
        writer.writeBootstrap();
    }

    for (auto const& in_filename : in_filenames) {
        ifstream inputfile(in_filename);
        if (!inputfile.is_open()) {
            std::cerr << "Could not open input file " << in_filename << endl;
            outfile.close();
            return 1;
        }

        Parser parser = Parser(inputfile);
        writer.setFilename(in_filename.filename());
        while (true) {
            parser.advance();
            parser.commandType();
            if (!parser.hasMoreCommands()) {
                break;
            }
            writer.write(parser.commandType(), parser.command(), parser.arg1(), parser.arg2());
        }
        inputfile.close();
    }

    outfile.close();
    return 0;
}
