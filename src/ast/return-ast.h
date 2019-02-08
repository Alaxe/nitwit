#pragma once
#include "assignment-ast.h"
#include "expr-ast.h"
#include "statement-ast.h"

class ReturnAST : public StatementAST {
private:
	AssignmentAST::UPtr assignment;
	uint32_t blockId;
public:
	ReturnAST(const Line &l, const Context &context);
	virtual void generate_c(std::ostream &out) const;
	virtual void generate_c(std::ostream &out, uint32_t indent) const;

};
