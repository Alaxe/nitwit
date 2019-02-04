#include "var-data.h"
#include <cassert>

VarData::VarData(const TypeT &type, std::string name):
	type(type), name(std::move(name)) {}

void VarData::c_name(std::ostream &out) const {
	out << "v_";
	out << name;
}
void VarData::c_declaration(std::ostream &out, uint32_t indent) const {
	out << std::string(indent, ' ');
	type.c_name(out);
	out << " ";
	c_name(out);
	out << " = ";
	if (dynamic_cast<const NonPrimitiveType*> (&type)) {
		out << "NULL";
	} else if (dynamic_cast<const PrimitiveType*> (&type)) {
		out << "0";
	} else {
		assert(false); //should not trigger
	}
	out << ";\n";
}
