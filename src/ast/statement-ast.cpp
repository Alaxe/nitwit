#include "statement-ast.h"

StatementAST::~StatementAST() {}
void StatementAST::debug_print() const {}
void StatementAST::generate_c(std::ostream &out, uint32_t indent) const {
	out << std::string(indent, ' ');
	generate_c(out);
}
