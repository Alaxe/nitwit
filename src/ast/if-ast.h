#pragma once

#include "statement-ast.h"
#include "expr-ast.h"

#include <utility>

class IfAST : public StatementAST {
private:
	typedef std::pair<ExprAST::UPtr, StatementAST::UPtr> IfPair;
	std::vector<IfPair> ifPairs;
	StatementAST::UPtr elseBody;

public:
	IfAST(Line::ConstIt &begin, Line::ConstIt end, Context &context);

	void generate_c(std::ostream &out) const;
	void generate_c(std::ostream &out, uint32_t indent) const;
	void debug_print() const;
};
