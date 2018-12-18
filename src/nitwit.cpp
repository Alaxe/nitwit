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
		std::cout << i.lineInd << " (" << i.indent << "): ";
		for (auto &t : i.tokens) {
			std::cout << "[" << (uint32_t) (t.type) << " " << t.s << "] ";
		}
		std::cout << "\n";
	}

	GlobalContext globalContext = GlobalContext(code);
	std::vector<Function> functions = Function::parse_all(code, globalContext);
	globalContext.generate_c(std::cout);
}
