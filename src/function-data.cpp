#include "function-data.h"

#include <cassert>
#include <iostream>

FunctionData::Argument::Argument(const TypeT &type, std::string name):
	type(type), name(std::move(name)) {}

FunctionData::FunctionData(
	const TypeT &returnT,
	std::string name,
	std::vector<Argument> args
): returnT(returnT), name(std::move(name)), args(std::move(args)) {}

void FunctionData::c_name(std::ostream &out) const {
	out << "f_" << name;
}

void FunctionData::generate_c(std::ostream &out) const {
	returnT.c_name(out);
	out << " ";
	c_name(out);
	out << "(";
	if (!args.empty()) {
		args[0].first.c_name(out);
		out << " " << args[0].second;
		for (uint32_t i = 1;i < args.size();i++) {
			out << ", ";
			args[i].type.c_name(out);
			out << " ";
			Context::get_c_arg_name(out, args[i].second);
		}
	} else {
		out << "void";
	}
	out << ")";
}
