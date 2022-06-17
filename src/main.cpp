// See what we can remove here
#include <cstdlib>
#include <iostream>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <sstream>
#include "parser.hpp"
#include "writer.hpp"
#include <execinfo.h>
#include <filesystem>

void print_trace (void)
{
  void *array[10];
  char **strings;
  int size, i;

  size = backtrace (array, 10);
  strings = backtrace_symbols (array, size);
  if (strings != NULL)
  {

    printf ("Obtained %d stack frames.\n", size);
    for (i = 0; i < size; i++)
      printf ("%s\n", strings[i]);
  }
  free (strings);
}

using namespace std;

// TODO: Better management of file handles
int main(int argc, char **argv)
{    
    // TODO: Validate one is Main.vm!
    // TODO: Validate one has main function!

	if (argc != 3)
	{
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
		if (found != string::npos)
		{
			filename.append(argv[2], found);
		}
		else
		{
			filename.append(argv[2]);
		}
		writer.setFilename(filename);
		while (true)
		{
			parser.advance();
			parser.commandType();
			if (!parser.hasMoreCommands())
			{
				break;
			}
			if (parser.commandType() == C_ARITHMETIC)
			{
				writer.writeArithemtic(parser.command());
			}
			else if (parser.commandType() == C_PUSH || parser.commandType() == C_POP)
			{
				writer.writePushPop(parser.commandType(), parser.arg1(), parser.arg2());
			}
			else if (parser.commandType() == C_LABEL)
			{
				writer.writeLabel(parser.commandType(), parser.arg1());
			}
			else if (parser.commandType() == C_IF)
			{
				writer.writeIf(parser.commandType(), parser.arg1());
			}
			else if (parser.commandType() == C_GOTO)
			{
				writer.writeGoto(parser.commandType(), parser.arg1());
			}
			else if (parser.commandType() == C_RETURN)
			{
                writer.writeReturn(parser.arg1(), parser.arg2());
			}
			else if (parser.commandType() == C_FUNCTION)
			{
				writer.writeFunction(parser.arg1(), parser.arg2());
			}
			else if (parser.commandType() == C_CALL)
			{
                writer.writeCall(parser.arg1(), parser.arg2());
			}
			else
			{
				// TODO: Exception
				cout << "Unsupported command type." << endl;
                return 1;
			}
		}
		inputfile.close();
    }

    outfile.close();
	return 0;
}
