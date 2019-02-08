#pragma once

#include "../context.h"
#include "../lexer.h"
#include "../type-t.h"

#include "statement-ast.h"

#include <memory>

class ExprAST : public StatementAST {
protected:
	bool lvalue;
	const TypeT *resultType;

public:
	typedef std::vector<std::unique_ptr<ExprAST>> Stack;
	typedef std::unique_ptr<ExprAST> UPtr;

	bool is_lvalue() const;
	const TypeT* get_result_type() const;

	using StatementAST::generate_c;
	virtual void generate_c(std::ostream &out) const;
	virtual void generate_expr(std::ostream &out) const = 0;

	static UPtr parse(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);
	static UPtr parse_condition(
		Token::ConstIt &begin,
		Token::ConstIt end,
		const Context &context
	);
};
