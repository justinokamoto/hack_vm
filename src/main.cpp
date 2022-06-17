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
    std::vector<string> in_filenames;
    if (std::filesystem::is_directory(argv[1])) {
        for (std::filesystem::path const& entry : std::filesystem::directory_iterator(argv[1])) {
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

        string filename;
        size_t found = string(argv[2]).find('.');
        if (found != string::npos) {
            filename.append(argv[2], found);
        } else {
            filename.append(argv[2]);
        }
        writer.setFilename(filename);
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
