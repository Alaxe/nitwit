#pragma once

#include "statement-ast.h"
#include "expr-ast.h"
#include "block-ast.h"

#include <utility>

class IfAST : public StatementAST {
private:
	typedef std::pair<ExprAST::UPtr, BlockAST::UPtr> IfPair;
	std::vector<IfPair> ifPairs;
	BlockAST::UPtr elseBody;

public:
	IfAST(Line::ConstIt &begin, Line::ConstIt end, Context &context);

	void generate_c(std::ostream &out) const;
	void generate_c(std::ostream &out, uint32_t indent) const;
};
