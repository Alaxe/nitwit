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
	type.c_default_value(out);
	out << ";\n";
}
void VarData::c_destruction(std::ostream &out, uint32_t indent) const {
	const auto *npt = dynamic_cast<const NonPrimitiveType*> (&type);
	if (npt == nullptr) {
		return;
	}
	std::string indentS = std::string(indent, ' ');

	out << indentS;
	npt->c_rm_ref_name(out);
	out << "(";
	c_name(out);
	out << ");\n";
}
void VarData::c_generate_destructors(
	std::ostream &out,
	const std::vector<VarData> &vars,
	uint32_t indent
) {
	for (const auto &i : vars) {
		i.c_destruction(out, indent);
	}
}

