#include "ast.h"
#include "prim-type-data.h"

#include <algorithm>
#include <cassert>
#include <iostream>

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


FunctionCallAST::FunctionCallAST(const Context &context,
	ExprAST::Stack &stack) {

	assert(!stack.empty());

	std::unique_ptr<IdentifierAST> funcIdent(
		dynamic_cast<IdentifierAST*> (stack.back().release())
	);
	stack.pop_back();
	assert(funcIdent != nullptr);
	funcName = funcIdent->get_name();


	const FunctionData *func = context.get_function(funcName);
	assert(func != nullptr);
	assert(func->args.size() <= stack.size());

	for (uint32_t i = 0;i < func->args.size();i++) {
		const ResultT &argT = stack.back()->get_result_t();
		assert(argT.is_value());
		assert(argT.t.cat == TypeT::Category::Primitive);

		args.push_back(std::move(stack.back()));
		stack.pop_back();
	}

	resultT.cat = ResultT::Category::RValue;
	resultT.t = func->returnT;
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
	uint32_t blIndent,
	std::vector<Line>::const_iterator& begin,
	std::vector<Line>::const_iterator end,
	Context &context,
	const FunctionData &func
): blIndent(blIndent) {
	context.update_indent(blIndent);
	while (begin != end) {
		const auto &tok = begin->tokens;
		if (begin->indent < blIndent) {
			break;
		} else if (begin->indent > blIndent) {
			statements.emplace_back(new BlockAST(
				begin->indent,
				begin,
				end,
				context,
				func
			));
			context.update_indent(blIndent);
			continue;
		}
		if (tok[0].type == TokenType::If) {
			auto ifAst = std::unique_ptr<IfAST>(new IfAST());

			TokenType nextIf = TokenType::If;

			while ((begin != end) && (begin->indent == blIndent)) {
				const auto &tok = begin->tokens;
				std::unique_ptr<BlockAST> body;

				if (
					(tok[0].type == nextIf)
					|| (tok[0].type == TokenType::Else)
				) {
					BlockAST *b = new BlockAST(
						blIndent + 1,
						++begin,
						end,
						context,
						func
					);
					body = std::unique_ptr<BlockAST>(b);
				}

				if (tok[0].type == nextIf) {
					auto cond = ExprAST::parse_condition(
						tok,
						context
					);
					ifAst->attach_if({
						std::move(cond), 
						std::move(body)
					});

					nextIf = TokenType::Elif;
				} else if (tok[0].type == TokenType::Else) {
					ifAst->attach_else_body(std::move(body));
					break;
				} else {
					break;
				}
			}

			statements.push_back(std::move(ifAst));
		} else if (
			(tok[0].type == TokenType::Else)
			|| (tok[0].type == TokenType::Elif)
		) {
			assert(false);
		} else if (tok[0].type == TokenType::VarDef) {
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
void BlockAST::generate_c(std::ostream &out, uint32_t indent) const {
	generate_c(out);
}
void BlockAST::generate_c(std::ostream &out) const {
	for (const auto &i : statements) {
		i->generate_c(out, blIndent);
	}
}


void IfAST::debug_print() const {}

void IfAST::attach_if(IfPair p) {
	ifPairs.push_back(std::move(p));
}
void IfAST::attach_else_body(std::unique_ptr<StatementAST> body) {
	elseBody = std::move(body);
}

void IfAST::generate_c(std::ostream &out) const {
	generate_c(out, 0);
}
void IfAST::generate_c(std::ostream &out, uint32_t indent) const {
	std::string indentS = std::string(indent, ' ');
	bool first = true;
	for (const IfPair &p : ifPairs) {
		out << (first ? indentS : " else ");
		out << "if (";
		p.first->generate_expr(out);
		out << ") {\n";
		p.second->generate_c(out);
		out << indentS << "}";

		first = false;
	}
	if (elseBody) {
		out << " else {\n";
		elseBody->generate_c(out);
		out << indentS << "}";
	}
	out << "\n";
}
