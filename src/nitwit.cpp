#include <algorithm>
#include <cassert>
#include <cctype>
#include <fstream>
#include <iostream>
#include <memory>
#include <unordered_map>
#include <vector>

#include "ast.h"
#include "context.h"
#include "function.h"
#include "lexer.h"
#include "operator-data.h"

int main() {
	std::ifstream fin("sample.ntwt");

	auto code = Line::split_stream(fin);
	for (auto &i : code) {
		i.tokens = Token::tokenize(i.s);
		std::cerr << i << "\n";
	}

	GlobalContext globalContext = GlobalContext(code);
	std::vector<Function> functions = Function::parse_all(code, globalContext);

	std::cout << "#include <stdio.h>\n";

	InputAST::generate_default_c(std::cout);
	OutputAST::generate_default_c(std::cout);

	globalContext.generate_c(std::cout);

	for (const auto &f : functions) {
		f.generate_c(std::cout);
	}
}
