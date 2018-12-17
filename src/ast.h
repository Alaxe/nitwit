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
	//a codegen virtual function
	virtual void debug_print() const;
};

class ResultT {
public:
	enum class Category {
		RValue,
		LValue,
		Identifier,
		Unknown
	};
	TypeT type;
	Category category;

	ResultT(Category cat = Category::Unknown);
	bool is_value() const;
};

class ExprAST : public StatementAST {
protected:
	ResultT resultType;
	typedef std::vector<std::unique_ptr<ExprAST>> Stack;

public:
	const ResultT& get_result_type();

	static std::vector<std::unique_ptr<ExprAST>> parse(
		std::vector<Token>::const_iterator begin,
		std::vector<Token>::const_iterator end,
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
};

class IdentifierAST : public ExprAST {
protected:
	std::string name;

public:
	IdentifierAST(const Token &t, const Context &context);
	const std::string& get_name() const;
	virtual void debug_print() const;
};

class BinOperatorAST : public ExprAST {
protected:
	const OperatorData *oprData;
	std::unique_ptr<ExprAST> lhs, rhs;

public:
	BinOperatorAST(const OperatorData *oprData, ExprAST::Stack &stack);
	virtual void debug_print() const;
};

class FunctionCallAST : public ExprAST {
protected:
	std::string funcName; //the nitwit one
	std::vector<std::unique_ptr<ExprAST>> args;

public:
	FunctionCallAST(const Context &context, ExprAST::Stack &stack);
	virtual void debug_print() const;
};

class AssignmentAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> lhs, rhs;
public:
	AssignmentAST(ExprAST::Stack &stack);

	virtual void debug_print() const;
};

class InputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	InputAST(ExprAST::Stack &stack); 
	virtual void debug_print() const;
};

class OutputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	OutputAST(ExprAST::Stack &stack);
	virtual void debug_print() const;
};


class VarDefAST : public StatementAST {
private:
	std::string name;
	TypeT type;
public:
	VarDefAST(const Token &typeTok, const Token &nameTok);

	void add_to_context(Context &context);
	virtual void debug_print() const;
};

class ReturnAST : public StatementAST {
private:
	std::unique_ptr<ExprAST> returnVal;
public:
	ReturnAST(std::unique_ptr<ExprAST> val);
	virtual void debug_print() const;
};
