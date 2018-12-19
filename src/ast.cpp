#include "ast.h"

#include <algorithm>
#include <cassert>
#include <iostream>

ResultT::ResultT(Category cat): category(cat) {}
bool ResultT::is_value() const {
	return (category == Category::RValue)
		|| (category == Category::LValue);
}

StatementAST::~StatementAST() {}

const ResultT& ExprAST::get_result_type() {
	return resultType;
}
void ExprAST::generate_c(std::ostream &out) const {
	out << "    ";
	generate_expr(out);
	out << ";\n";
}
std::vector<std::unique_ptr<ExprAST>> ExprAST::parse(
	std::vector<Token>::const_iterator begin,
	std::vector<Token>::const_iterator end,
	const Context &context) {

	std::vector<std::unique_ptr<ExprAST>> stack;
	for (auto it = end - 1;it + 1 != begin;it--) {
		const Token &tok = *it;
		if (tok.type == TokenType::LitInt) {
			stack.emplace_back(new LitAST(tok));
		} else if (tok.type == TokenType::Identifier) {
			stack.emplace_back(new IdentifierAST(tok, context));
		} else {
			assert(tok.type == TokenType::Operator);
			const OperatorData* oprData = OperatorData::get(tok.s);

			if (oprData != nullptr) {
				if (oprData->arity == 2) {
					stack.emplace_back(new BinOperatorAST(oprData, stack));
				} else {
					assert(oprData->arity == 2);
				}
			} else if (tok.s == "@") {
				stack.emplace_back(new FunctionCallAST(context, stack));
			} else if (tok.s == "=") {
				stack.emplace_back(new AssignmentAST(stack));
			} else if (tok.s == ">>") {
				stack.emplace_back(new InputAST(stack));
			} else if (tok.s == "<<") {
				stack.emplace_back(new OutputAST(stack));
			}
		}
	}

	std::reverse(stack.begin(), stack.end());
	return stack;
}

LitAST::LitAST(const Token &t): val(t.s) {
	resultType.category = ResultT::Category::RValue;
}
const std::string& LitAST::get_val() const {
	return val;
}
void LitAST::debug_print() const {
	std::cerr << "Literal " << val << "\n";
}
void LitAST::generate_expr(std::ostream &out) const {
	out << val;
}

IdentifierAST::IdentifierAST(const Token &t, const Context &context):
	name(t.s) {

	const TypeT *typePtr = context.get_variable(t.s);
	if (typePtr == nullptr) {
		resultType.category = ResultT::Category::Identifier;
	} else {
		resultType.category = ResultT::Category::LValue;
	}
}
const std::string& IdentifierAST::get_name() const {
	return name;
}
void IdentifierAST::debug_print() const {
	std::cerr << "Identifier ";
	if (resultType.category == ResultT::Category::LValue) {
		std::cerr << "(LValue) ";
	}
	std::cerr << name << "\n";
}
void IdentifierAST::generate_expr(std::ostream &out) const {
	out << name;
}

BinOperatorAST::BinOperatorAST(const OperatorData *oprData,
	ExprAST::Stack &stack): oprData(oprData) {

	assert((oprData != nullptr) && (oprData->arity == 2));

	assert(stack.size() >= 2);
	lhs = std::move(stack.back());
	stack.pop_back();
	rhs = std::move(stack.back());
	stack.pop_back();

	//It's a normal operator, so it only requires its arguments to
	//values or references

	resultType.category = ResultT::Category::RValue;

	assert(lhs->get_result_type().is_value());
	assert(rhs->get_result_type().is_value());
}
void BinOperatorAST::debug_print() const {
	std::cerr << "binary operator " << oprData->name << "\n";
	std::cerr << "first argument (\n";
	lhs->debug_print();
	std::cerr << ")\n";
	std::cerr << "second argument " << oprData->name << "(\n";
	rhs->debug_print();
	std::cerr << ")\n";
}
void BinOperatorAST::generate_expr(std::ostream &out) const {
	out << "(";
	lhs->generate_expr(out);
	out << ") " << oprData->cName << " (";
	rhs->generate_expr(out);
	out << ")";
}

FunctionCallAST::FunctionCallAST(const Context &context,
	ExprAST::Stack &stack) {

	assert(!stack.empty());

	std::unique_ptr<IdentifierAST> funcIdent(
		dynamic_cast<IdentifierAST*> (stack.back().release())
	);
	stack.pop_back();
	assert(funcIdent != nullptr);
	funcName = funcIdent->get_name();


	const FunctionT *funcT = context.get_function(funcName);
	assert(funcT != nullptr);
	assert(funcT->argCnt <= stack.size());

	resultType.category = ResultT::Category::RValue;

	for (uint32_t i = 0;i < funcT->argCnt;i++) {
		const ResultT &argT = stack.back()->get_result_type();
		assert(argT.is_value());
		//add a more thorough type check when, well, we have types

		args.push_back(std::move(stack.back()));
		stack.pop_back();
	}
}
void FunctionCallAST::debug_print() const {
	std::cerr << "function call " << funcName << " with ";
	std::cerr << args.size() << "arguments.\n";
	for (uint32_t i = 0;i < args.size();i++) {
		std::cerr << "argument " << i + 1 << " (";
		std::cerr << funcName << ") (\n";

		args[i]->debug_print();
		std::cerr << ")\n";
	}
}
void FunctionCallAST::generate_expr(std::ostream &out) const {
	out << funcName << "(";
	if (!args.empty()) {
		args[0]->generate_expr(out);
		for (uint32_t i = 1;i < args.size();i++) {
			out << ", ";
			args[i]->generate_expr(out);
		}
	}
	out << ")";
}

AssignmentAST::AssignmentAST(ExprAST::Stack &stack) {
	assert (stack.size() >= 2);
	lhs = std::move(stack.back());
	stack.pop_back();
	rhs = std::move(stack.back());
	stack.pop_back();

	assert(lhs->get_result_type().category == ResultT::Category::LValue);
	assert(rhs->get_result_type().is_value());

	resultType.category = ResultT::Category::RValue;
}
void AssignmentAST::debug_print() const {
	std::cerr << "assignment \n";
	std::cerr << "first argument (\n";
	lhs->debug_print();
	std::cerr << ")\n";
	std::cerr << "second argument (assignment) (\n";
	rhs->debug_print();
	std::cerr << ")\n";
}
void AssignmentAST::generate_expr(std::ostream &out) const {
	out << "(";
	lhs->generate_expr(out);
	out << ") = (";
	rhs->generate_expr(out);
	out << ")";
}

InputAST::InputAST(ExprAST::Stack &stack) {
	assert (!stack.empty());
	operand = std::move(stack.back());
	stack.pop_back();

	assert(operand->get_result_type().category == ResultT::Category::LValue);
	resultType.category = ResultT::Category::RValue;
}
void InputAST::debug_print() const {
	std::cerr << "input >> (\n";
	operand->debug_print();
	std::cerr << ")\n";
}
void InputAST::generate_expr(std::ostream &out) const {
	out << "in_int( &(";
	operand->generate_expr(out);
	out << "))";
}

void InputAST::generate_default_c(std::ostream &out) {
	out << "int in_int(int* a) {\n";
	out << "    scanf(\"%i\", a);\n";
	out << "    return *a;\n";
	out << "}\n";
}

OutputAST::OutputAST(ExprAST::Stack &stack) {
	assert (!stack.empty());
	operand = std::move(stack.back());
	stack.pop_back();

	assert(operand->get_result_type().is_value());
	resultType.category = ResultT::Category::RValue;
}
void OutputAST::debug_print() const {
	std::cerr << "output >> (\n";
	operand->debug_print();
	std::cerr << ")\n";
}
void OutputAST::generate_expr(std::ostream &out) const {
	out << "out_int(";
	operand->generate_expr(out);
	out << ")";
}

void OutputAST::generate_default_c(std::ostream &out) {
	out << "int out_int(int a) {\n";
	out << "    printf(\"%i\\n\", a);\n";
	out << "    return a;\n";
	out << "}\n";
}

VarDefAST::VarDefAST(const Token &typeTok, const Token &nameTok) {
	assert(typeTok.type == TokenType::Identifier);
	assert(typeTok.s == "int");
	assert(nameTok.type == TokenType::Identifier);

	name = nameTok.s;
}
void VarDefAST::add_to_context(Context &context) {
	context.declare_variable(name, type);
}
void VarDefAST::debug_print() const {
	std::cerr << "vardef (no type data) " << name << "\n";
}
void VarDefAST::generate_c(std::ostream &out) const {
	out << "    " << name << " = 0;\n";
}

ReturnAST::ReturnAST(std::unique_ptr<ExprAST> val): returnVal(std::move(val)) {
	assert(returnVal->get_result_type().is_value());
}
void ReturnAST::debug_print() const {
	std::cerr << "Return (\n";
	returnVal->debug_print();
	std::cerr << ")\n";
}
void ReturnAST::generate_c(std::ostream &out) const {
	out << "    return ";
	returnVal->generate_expr(out);
	out << ";\n";
}
