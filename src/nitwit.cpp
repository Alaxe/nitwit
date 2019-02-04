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

	GlobalContext globalContext = GlobalContext(code.begin(), code.end());
	std::vector<Function> functions = Function::parse_all(
		code.begin(),
		code.end(),
		globalContext
	);

	std::ofstream fout("sample.c");
	fout << "#include <stdio.h>\n";
	fout << "#include <inttypes.h>\n";

	globalContext.generate_c(fout);

	for (const auto &f : functions) {
		f.generate_c(fout);
	}
}
