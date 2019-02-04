#include "primitive-type.h"

const std::vector<PrimitiveType> PrimitiveType::types = {
	{"real",   true,  0,  "double",   "\"lf\"", "\"f\""},
	{"float",  true,  1,  "float",    "\"f\"",  "\"f\""},
	{"uint64", false, 2,  "uint64_t", "SCNu64", "PRIu64"},
	{"int64",  false, 3,  "int64_t",  "SCNi64", "PRIi64"},
	{"uint32", false, 4,  "uint32_t", "SCNu32", "PRIu32"},
	{"uint",   false, 5,  "uint32_t", "SCNu32", "PRIu32"},
	{"int32",  false, 6,  "int32_t",  "SCNi32", "PRIi32"},
	{"int",    false, 7,  "int32_t",  "SCNi32", "PRIi32"},
	{"uint16", false, 8,  "uint16_t", "SCNu16", "PRIu16"},
	{"int16",  false, 9,  "int16_t",  "SCNi16", "PRIi16"},
	{"uint8",  false, 10, "uint8_t",  "SCNu8",  "PRIu8"},
	{"int8",   false, 11, "int8_t",   "SCNi8",  "PRIi8"}
};

const std::string PrimitiveType::maxInt = "int64";
const std::string PrimitiveType::maxFloat = "real";
const std::string PrimitiveType::defaultInt = "int";

PrimitiveType::PrimitiveType(
	const std::string &name,
	bool isFloat,
	uint32_t castIndex,
	std::string cName,
	std::string scanfMacro,
	std::string printfMacro
):
	name(name),
	isFloat(isFloat),
	castIndex(castIndex),
	cName(std::move(cName)),
	scanfMacro(std::move(scanfMacro)),
	printfMacro(std::move(printfMacro))
{}

const PrimitiveType& PrimitiveType::common_type(const PrimitiveType &t2) const {
	return t2.castIndex < castIndex ? t2 : *this;
}
void PrimitiveType::c_read_function(std::ostream &s) const {
	s << "read_" << name;
}
void PrimitiveType::c_write_function(std::ostream &s) const {
	s << "write_" << name;
}

bool PrimitiveType::assignable(const TypeT &a) const {
	return dynamic_cast<const PrimitiveType*>(&a) != nullptr;
}
bool PrimitiveType::is_declarable() const {
	return true;
}
void PrimitiveType::get_name(std::ostream &s) const {
	s << name;
}
void PrimitiveType::c_declare_type(std::ostream &s) const {
	if (castIndex == 0) {
		s << "#include <inttypes.h>\n";
		s << "#include <stdio.h>\n";
	}
}
void PrimitiveType::c_define_type(std::ostream &s) const {
	s << "int32_t ";
	c_read_function(s);
	s << "(" << cName << " *a) {\n";
	s << "\treturn scanf(\"%\" " << scanfMacro << ", a) > 0 ? 1 : 0;\n";
	s << "}\n";

	s << "int32_t ";
	c_write_function(s);
	s << "(" << cName << " a) {\n";
	s << "\treturn printf(\"%\" " << printfMacro << " \"\\n\", a) > 0 ? 1 : 0;\n";
	s << "}\n";
}
void PrimitiveType::c_name(std::ostream &s) const {
	s << cName;
}
