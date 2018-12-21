#include "function-t.h"
#include "prim-type-data.h"

#include <cassert>
#include <iostream>

FunctionT::FunctionT() {}

FunctionT::FunctionT(
	const TypeT &returnT,
	const std::string &name,
	const std::vector<ArgT> &args
): returnT(returnT), name(name), args(args) {}

FunctionT::FunctionT(const Line &l) {
	if ((l.tokens.size() < 3) || (l.tokens.size() % 2 == 0)) {
		std::cerr << "Invalid number of tokens for ";
		std::cerr << "func def on line ";
		std::cerr << l.lineInd << "\n";

		assert(false);
	}

	const Token &retTok = l.tokens[1];
	const Token &nameTok = l.tokens[2];

	assert(retTok.type == TokenType::Identifier);
	assert(nameTok.type == TokenType::Identifier);

	if (retTok.s == "void") {
		returnT.cat = TypeT::Category::Void;
	} else if (PrimTypeData::get(retTok.s)) {
		returnT = TypeT(TypeT::Category::Primitive, retTok.s);
	} else {
		std::cerr << "Invalid function type " << retTok.s << "\n";
		assert(false);
	}

	name = nameTok.s;
	FunctionT::ArgT curArg;
	for (uint32_t i = 3;i < l.tokens.size();i++) {
		const Token &t = l.tokens[i];

		assert(t.type == TokenType::Identifier);
		if (i % 2 == 1) {
			assert(PrimTypeData::get(t.s));
			curArg.first = TypeT(TypeT::Category::Primitive, t.s);
		} else {
			curArg.second = t.s;
			args.push_back(std::move(curArg));
		}
	}
}

void FunctionT::generate_c(std::ostream &out) const {
	out << returnT.get_c_name() << " " << name << "(";
	if (!args.empty()) {
		out << args[0].first.get_c_name() << " " << args[0].second;
		for (uint32_t i = 1;i < args.size();i++) {
			out << ", " << args[i].first.get_c_name();
			out << " " << args[i].second;
		}
	} else {
		out << "void";
	}
	out << ")";
}
