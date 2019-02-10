#include "result-ast.h"

const TypeT* ResultAST::return_t(const Context &context) {
	return &context.functionData.returnT;
}
const std::string ResultAST::name = "result";

ResultAST::ResultAST(const Context &context) {
	lvalue = true;
	resultType = return_t(context);
}
void ResultAST::generate_expr(std::ostream &out) const {
	out << name;
}

void ResultAST::c_declare_result(
	std::ostream &out,
	const TypeT &retT,
	uint32_t indent
) {
	if (!retT.is_declarable()) {
		return;
	}

	out << std::string(indent, ' ');
	retT.c_name(out);
	out << " " << name << " = ";
	retT.c_default_value(out);
	out << ";\n";
}

void ResultAST::c_return_result(
	std::ostream &out,
	const TypeT &retT,
	uint32_t indent
) {
	out << std::string(indent, ' ');
	if (retT.is_declarable()) {
		out << "return " << name;
	}
	out << ";\n"; //the null statement for void function labels
}
