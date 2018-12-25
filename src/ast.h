#pragma once

#include <memory>
#include <vector>

#include "context.h"
#include "lexer.h"
#include "operator-data.h"
#include "type-t.h"

class StatementAST {
public:
	virtual ~StatementAST();
	virtual void debug_print() const = 0;
	virtual void generate_c(std::ostream &out) const = 0;
	virtual void generate_c(std::ostream &out, uint32_t indent) const;
};

class ResultT {
public:
	enum class Category {
		RValue,
		LValue,
		Identifier,
		Unknown
	};
	Category cat;
	TypeT t;

	ResultT(Category cat = Category::Unknown);
	bool is_value() const;
};

class ExprAST : public StatementAST {
protected:
	ResultT resultT;

public:
	typedef std::vector<std::unique_ptr<ExprAST>> Stack;
	const ResultT& get_result_t();

	void generate_c(std::ostream &out) const;
	virtual void generate_expr(std::ostream &out) const = 0;

	static std::vector<std::unique_ptr<ExprAST>> parse(
		std::vector<Token>::const_iterator begin,
		std::vector<Token>::const_iterator end,
		const Context &context
	);
	static std::unique_ptr<ExprAST> parse_condition(
		const std::vector<Token> &tok,
		const Context &context
	);
};

class UnOperatorAST : public ExprAST {
protected:
	std::string opr;
	std::unique_ptr<ExprAST> operand;
};

class LitAST : public ExprAST {
private:
	std::string val;
public:
	LitAST(const Token &t);
	const std::string& get_val() const;
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};

class IdentifierAST : public ExprAST {
protected:
	std::string name;

public:
	IdentifierAST(const Token &t, const Context &context);
	const std::string& get_name() const;
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};

class BinOperatorAST : public ExprAST {
protected:
	const OperatorData *oprData;
	std::unique_ptr<ExprAST> lhs, rhs;



public:
	BinOperatorAST(const OperatorData *oprData, ExprAST::Stack &stack);
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};

class FunctionCallAST : public ExprAST {
protected:
	std::string funcName; //the nitwit one
	std::vector<std::unique_ptr<ExprAST>> args;

public:
	FunctionCallAST(const Context &context, ExprAST::Stack &stack);
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};

class AssignmentAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> lhs, rhs;
public:
	AssignmentAST(ExprAST::Stack &stack);

	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;
};

class InputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	InputAST(ExprAST::Stack &stack);
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;

	static void generate_default_c(std::ostream &out);
};

class OutputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	OutputAST(ExprAST::Stack &stack);
	virtual void debug_print() const;
	virtual void generate_expr(std::ostream &out) const;

	static void generate_default_c(std::ostream &out);
};

class ReturnAST : public StatementAST {
private:
	std::unique_ptr<ExprAST> returnVal;
public:
	ReturnAST(std::unique_ptr<ExprAST> val);
	virtual void debug_print() const;
	virtual void generate_c(std::ostream &out) const;
};

class BlockAST : public StatementAST {
private:
	uint32_t blIndent;
	std::vector<std::unique_ptr<StatementAST>> statements;
public:
	BlockAST(
		uint32_t blIndent,
		std::vector<Line>::const_iterator &begin,
		std::vector<Line>::const_iterator end,
		Context &context,
		const FunctionT &func
	);
	void debug_print() const;
	void generate_c(std::ostream &out) const;
	void generate_c(std::ostream &out, uint32_t indent) const;
};

class IfAST : public StatementAST {
public:
	typedef std::pair<
		std::unique_ptr<ExprAST>,
		std::unique_ptr<StatementAST>
	> IfPair;
private:
	std::vector<IfPair> ifPairs;
	std::unique_ptr<StatementAST> elseBody;
public:
	void attach_if(IfPair p);
	void attach_else_body(std::unique_ptr<StatementAST> body);

	void generate_c(std::ostream &out) const;
	void generate_c(std::ostream &out, uint32_t indent) const;
	void debug_print() const;
};
