#include "struct-type.h"

StructType::StructType(const StructData &data): StructType(data, *this) {}
StructType::StructType(const StructData &data, const StructType &rvalueT):
	NonPrimitiveType(rvalueT), data(data) {}

const StructType& StructType::rvalue_conversion() const {
	return static_cast<const StructType&> (rvalueT);
}


std::pair<StructType::UPtr, StructType::UPtr> StructType::make_pair(
	const StructData &data
) {
	StructType::UPtr strong(new StructType(data));
	StructType::UPtr weak(new StructType(data, *strong));

	return {std::move(strong), std::move(weak)};
}
void StructType::get_name(std::ostream &s) const {
	if (weak()) {
		s << '~';
	}
	s << data.name;
}
void StructType::c_name_raw(std::ostream &s) const {
	s << "s_" << data.name;
}
void StructType::c_declare_type(std::ostream &s) const {
	if (weak()) {
		return;
	}

	s << "struct ";
	c_name_raw(s);
	s << ";\n";
}
void StructType::c_define_type(std::ostream &s) const {
	if (weak()) {
		return;
	}
}

void StructType::c_member_name(std::ostream &s,const std::string &name) {
	s << "m_" << name;
}

