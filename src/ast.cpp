#include "ast.h"
#include "prim-type-data.h"

#include <algorithm>
#include <cassert>
#include <iostream>

ResultT::ResultT(Category cat): cat(cat) {}
bool ResultT::is_value() const {
	return (cat == Category::RValue)
		|| (cat == Category::LValue);
}

StatementAST::~StatementAST() {}

const ResultT& ExprAST::get_result_t() {
	return resultT;
}
void ExprAST::generate_c(std::ostream &out) const {
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
		switch (tok.type) {
		case TokenType::LitInt:
		case TokenType::LitFloat:
			stack.emplace_back(new LitAST(tok));
			break;
		case TokenType::Identifier:
			stack.emplace_back(new IdentifierAST(tok, context));
			break;
		default:
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
	resultT.cat = ResultT::Category::RValue;
	resultT.t.cat = TypeT::Category::Primitive;

	if (t.type == TokenType::LitInt) {
		resultT.t.name = PrimTypeData::get_max_int();
	} else if (t.type == TokenType::LitFloat) {
		resultT.t.name = PrimTypeData::get_max_float();
	} else {
		assert(false);
	}

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

IdentifierAST::IdentifierAST(const Token &t, const Context &context) {

	const VarData *varData = context.get_variable(t.s);
	if (varData == nullptr) {
		resultT.cat = ResultT::Category::Identifier;
		name = t.s;
	} else {
		resultT.cat = ResultT::Category::LValue;
		resultT.t = varData->type;
		name = varData->name;
	}
}
const std::string& IdentifierAST::get_name() const {
	return name;
}
void IdentifierAST::debug_print() const {
	std::cerr << "Identifier ";
	if (resultT.cat == ResultT::Category::LValue) {
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


	const ResultT &lRes = lhs->get_result_t();
	const ResultT &rRes = rhs->get_result_t();
	assert(lRes.is_value());
	assert(rRes.is_value());
	assert(rRes.t.cat == TypeT::Category::Primitive);
	assert(lRes.t.cat == TypeT::Category::Primitive);

	const PrimTypeData *lResData = PrimTypeData::get(lRes.t.name);
	const PrimTypeData *rResData = PrimTypeData::get(rRes.t.name);

	if (!oprData->takeFloat) {
		assert(!lResData->isFloat);
		assert(!rResData->isFloat);
	}

	resultT.cat = ResultT::Category::RValue;
	resultT.t.cat = TypeT::Category::Primitive;
	resultT.t.name = PrimTypeData::combine(
		PrimTypeData::get(lRes.t.name),
		PrimTypeData::get(rRes.t.name)
	)->name;

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
	assert(funcT->args.size() <= stack.size());

	for (uint32_t i = 0;i < funcT->args.size();i++) {
		const ResultT &argT = stack.back()->get_result_t();
		assert(argT.is_value());
		assert(argT.t.cat == TypeT::Category::Primitive);

		args.push_back(std::move(stack.back()));
		stack.pop_back();
	}

	resultT.cat = ResultT::Category::RValue;
	resultT.t = funcT->returnT;
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

	assert(lhs->get_result_t().cat == ResultT::Category::LValue);
	assert(rhs->get_result_t().is_value());

	resultT.cat = ResultT::Category::RValue;
	resultT.t = lhs->get_result_t().t;
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

	assert(operand->get_result_t().cat == ResultT::Category::LValue);
	assert(operand->get_result_t().t.cat == TypeT::Category::Primitive);
	resultT.cat = ResultT::Category::RValue;
	resultT.t = operand->get_result_t().t;
}
void InputAST::debug_print() const {
	std::cerr << "input >> (\n";
	operand->debug_print();
	std::cerr << ")\n";
}
void InputAST::generate_expr(std::ostream &out) const {
	out << "in_" << resultT.t.name << "(&(";
	operand->generate_expr(out);
	out << "))";
}

void InputAST::generate_default_c(std::ostream &out) {
	for (const auto &i : PrimTypeData::get_all()) {
		const PrimTypeData &d = i.second;
		out << d.cName << " in_" << d.name;
		out << "(" << d.cName << "* a) {\n";
		out << "    scanf(\"%\" " << d.scanfMacro << ", a);\n";
		out << "    return *a;\n";
		out << "}\n";
	}
}

OutputAST::OutputAST(ExprAST::Stack &stack) {
	assert (!stack.empty());
	operand = std::move(stack.back());
	stack.pop_back();

	assert(operand->get_result_t().is_value());
	assert(operand->get_result_t().t.cat == TypeT::Category::Primitive);

	resultT.cat = ResultT::Category::RValue;
	resultT.t = operand->get_result_t().t;
}
void OutputAST::debug_print() const {
	std::cerr << "output >> (\n";
	operand->debug_print();
	std::cerr << ")\n";
}
void OutputAST::generate_expr(std::ostream &out) const {
	out << "out_" << resultT.t.name << "(";
	operand->generate_expr(out);
	out << ")";
}

void OutputAST::generate_default_c(std::ostream &out) {
	for (const auto &i : PrimTypeData::get_all()) {
		const PrimTypeData &d = i.second;
		out << d.cName << " out_" << d.name;
		out << "(" << d.cName << " a) {\n";
		out << "    printf(\"%\" " << d.printfMacro;
		out << " \"\\n\", a);\n";
		out << "    return a;\n";
		out << "}\n";
	}

}

ReturnAST::ReturnAST(std::unique_ptr<ExprAST> val): returnVal(std::move(val)) {}

void ReturnAST::debug_print() const {
	std::cerr << "Return (\n";
	returnVal->debug_print();
	std::cerr << ")\n";
}
void ReturnAST::generate_c(std::ostream &out) const {
	out << "return ";
	returnVal->generate_expr(out);
	out << ";\n";
}

BlockAST::BlockAST(
	uint32_t indent,
	std::vector<Line>::const_iterator& begin,
	std::vector<Line>::const_iterator end,
	Context &context,
	const FunctionT &func
): indent(indent) {
	context.update_indent(indent);
	while (begin != end) {
		const auto &tok = begin->tokens;
		if (begin->indent < indent) {
			break;
		} else if (begin->indent > indent) {
			statements.emplace_back(new BlockAST(
				begin->indent,
				begin,
				end,
				context,
				func
			));
			context.update_indent(indent);
			continue;
		}
		if (tok[0].type == TokenType::DefVar) {
			assert(tok.size() >= 3);

			assert(tok[1].type == TokenType::Identifier);
			assert(tok[2].type == TokenType::Identifier);


			assert(tok[1].s != "void");
			assert(PrimTypeData::get(tok[1].s));

			TypeT type = TypeT(
				TypeT::Category::Primitive,
				tok[1].s
			);

			ExprAST::Stack st;
			if (tok.size() > 3) {
				st = ExprAST::parse(
					tok.begin() + 3,
					tok.end(),
					context
				);
				assert(st.size() == 1);
			} else {
				Token tok;
				tok.type = TokenType::LitInt;
				tok.s = "0";
				st.emplace_back(new LitAST(tok));
			}

			context.declare_variable(tok[2].s, type);
			st.emplace_back(new IdentifierAST(tok[2], context));

			statements.emplace_back(new AssignmentAST(st));
			begin++;
		} else if (tok[0].type == TokenType::Return) {
			if (func.returnT.cat == TypeT::Category::Void) {
				assert(tok.size() == 1);
				continue;
			}
			auto expr = ExprAST::parse(
				tok.begin() + 1,
				tok.end(),
				context
			);

			assert(expr.size() == 1);
			const ResultT &retT = expr[0]->get_result_t();
			assert(retT.is_value());
			assert(retT.t.cat == TypeT::Category::Primitive);

			std::unique_ptr<ReturnAST> ptr(
				new ReturnAST(std::move(expr[0]))
			);

			statements.push_back(std::move(ptr));
			begin++;
		} else {
			auto expr = ExprAST::parse(
				tok.begin(),
				tok.end(),
				context
			);

			for (auto &i : expr) {
				assert(i->get_result_t().is_value());
				std::unique_ptr<StatementAST> ptr(
					static_cast<StatementAST*>(i.release())
				);
				statements.push_back(std::move(ptr));
			}
			begin++;
		}
	}
}

void BlockAST::debug_print() const {}
void BlockAST::generate_c(std::ostream &out) const {
	for (const auto &i : statements) { 
		if (dynamic_cast<BlockAST*>(i.get()) == nullptr) {
			out << std::string(indent, ' ');
		}
		i->generate_c(out);
	}
}
