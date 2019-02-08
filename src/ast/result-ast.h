#pragma once

#include "expr-ast.h"

class ResultAST : public ExprAST {
protected:
	static const TypeT *return_t(const Context &context);
public:
	static const std::string name;
	ResultAST(const Context &context);

	void generate_expr(std::ostream &out) const;

	static void c_declare_result(
		std::ostream &out,
		const TypeT &retT,
		uint32_t indent = 0
	);
	static void c_return_result(
		std::ostream &out,
		const TypeT &retT,
		uint32_t indent = 0
	);
};
