#include "function.h"
#include "prim-type-data.h"

#include <cassert>
#include <iostream>

Function::Function(
	std::vector<Line>::const_iterator begin,
	std::vector<Line>::const_iterator end,
	const GlobalContext &globalContext
): proto(*begin) {
	Context context(globalContext);

	for (auto &i : proto.args) {
		context.declare_variable(i.second, i.first);
		i.second = context.get_variable(i.second)->name;
	}

	for (auto lineIt = begin + 1;lineIt != end;lineIt++) {
		auto tok = lineIt->tokens;
		context.update_indent(lineIt->indent);
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
		} else if (tok[0].type == TokenType::Return) {
			if (proto.returnT.cat == TypeT::Category::Void) {
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
		}
		//statements.back()->debug_print();
		//std::cerr << "-----------------" << std::endl;
	}
	declarations = context.get_declarations();
}

void Function::generate_c(std::ostream &out) const {
	proto.generate_c(out);
	out << " {\n";
	for (const VarData &i : declarations) {
		out << "    ";
		i.generate_c(out);
	}

	for (const auto &i : statements) {
		i->generate_c(out);
	}
	out << "}\n";
}

std::vector<Function> Function::parse_all(
		const std::vector<Line> &code,
		const GlobalContext &globalContext) {

	std::vector<Function> functions;
	for (uint32_t i = 0;i < code.size();i++) {
		assert(code[i].indent == 0);
		if (code[i].tokens[0].type == TokenType::DefVar) {
			continue;
		}
		assert(code[i].tokens[0].type == TokenType::DefFunc);
		uint32_t j;
		for (j = i + 1;j < code.size();j++) {
			if (code[j].indent == 0) {
				break;
			}
		}
		functions.push_back(
			Function(code.begin() + i, code.begin() + j, globalContext)
		);
		i = j - 1;
	}
	return functions;
}
