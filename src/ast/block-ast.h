#pragma once
#include "statement-ast.h"
#include "../lexer.h"
#include "../context.h"

class BlockAST : public StatementAST {
private:
	uint32_t indent;
	uint32_t id;
	uint32_t parId;

	std::vector<StatementAST::UPtr> statements;
	std::vector<VarData> declarations;
public:
	BlockAST(
		Line::ConstIt &begin,
		Line::ConstIt end,
		Context &context,
		uint32_t indent = 1
	);
	void generate_c(std::ostream &out) const;
	void generate_c(std::ostream &out, uint32_t parIndent) const;
};
