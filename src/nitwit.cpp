#include "program.h"
#include <cassert>
#include <fstream>

int main(int argc, char** argv) {
	if (argc != 3) {
		std::cerr << "Usage ./nitwit <source file> <output file>\n";
		return 1;
	}
	std::ifstream sourceFile(argv[1]);
	if (sourceFile.fail()) {
		std::cerr << "Could not open source file: " << argv[1] << "\n";
		return 1;
	}
	std::ofstream outputFile(argv[2]);
	if (outputFile.fail()) {
		std::cerr << "Could not open output file: " << argv[2] << "\n";
		return 1;
	}

	Program program(sourceFile);
	program.generate_c(outputFile);
}
