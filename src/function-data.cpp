#include "function-data.h"
#include "prim-type-data.h"

#include <cassert>
#include <iostream>

FunctionData::FunctionData() {}

FunctionData::FunctionData(
	const TypeT &returnT,
	const std::string &name,
	const std::vector<ArgT> &args
): returnT(returnT), name(name), args(args) {}

void FunctionData::c_name(std::ostream &out) const {
	out << "f_" << name;
}

void FunctionData::generate_c(std::ostream &out) const {
	out << returnT.get_c_name() << " ";
	c_name(out);
	out << "(";
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
