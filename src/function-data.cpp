#include "function-data.h"

#include <cassert>
#include <iostream>

FunctionData::FunctionData(
	const TypeT &returnT,
	std::string name,
	std::vector<VarData> args
): returnT(returnT), name(std::move(name)), args(std::move(args)) {}

void FunctionData::c_name(std::ostream &out) const {
	out << "f_" << name;
}

void FunctionData::c_prototype(std::ostream &out) const {
	returnT.c_name(out);
	out << " ";
	c_name(out);
	out << "(";
	if (!args.empty()) {
		args[0].type.c_name(out);
		out << " ";
		args[0].c_name(out);
		for (uint32_t i = 1;i < args.size();i++) {
			out << ", ";
			args[i].type.c_name(out);
			out << " ";
			args[i].c_name(out);
		}
	} else {
		out << "void";
	}
	out << ")";
}
