#include "program.h"

#include <cassert>

Program::Program(std::istream &s) {
	auto code = Line::split_stream(s);
	for (auto &i : code) {
		i.tokens = Token::tokenize(i.s);
	}

	globalContext = GlobalContext(code.begin(), code.end());
	functions = Function::parse_all(
		code.begin(),
		code.end(),
		globalContext
	);
}

void Program::generate_c(std::ostream &s) const {
	s << "#include <stdlib.h>\n";
	globalContext.generate_c(s);

	for (const auto &f : functions) {
		f.generate_c(s);
	}
	const FunctionData *mainData = globalContext.get_function("main");

	assert(mainData);
	assert(&mainData->returnT ==
		globalContext.get_type(PrimitiveType::defaultInt)
	);
	assert(mainData->args.empty());
	s << "int main()\n{\n";
	mainData->c_name(s);

	s << "();\n}\n";

}
