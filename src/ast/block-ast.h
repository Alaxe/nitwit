#pragma once
#include "statement-ast.h"
#include "../lexer.h"
#include "../context.h"

class BlockAST : public StatementAST {
private:
	uint32_t indent;
	std::vector<StatementAST::UPtr> statements;
public:
	BlockAST(
		Line::ConstIt &begin,
		Line::ConstIt end,
		Context &context,
		uint32_t indent = 1
	);
	void generate_c(std::ostream &out) const;
	void generate_c(std::ostream &out, uint32_t indent) const;
};
