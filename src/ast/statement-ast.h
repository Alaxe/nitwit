#pragma once

#include <ostream>
#include <memory>

class StatementAST {
public:
	typedef std::unique_ptr<StatementAST> UPtr;
	virtual ~StatementAST();
	virtual void debug_print() const;
	virtual void generate_c(std::ostream &out) const = 0;
	virtual void generate_c(std::ostream &out, uint32_t indent) const;
};
