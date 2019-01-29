#include "struct-type.h"

const StructType* StructType::rvalue_conversion() const {
	return static_cast<const StructType*> (rvalueT);
}

std::pair<StructType::UPtr, StructType::UPtr> StructType::make_pair(
	const StructData *data
) {
	StructType::UPtr strong(new StructType());
	StructType::UPtr weak(new StructType());

	strong->data = weak->data = data;
	strong->rvalueT = strong->rvalueT;
	weak->rvalueT = strong;

	return {std::move(strong), std::move(weak)};
}
std::ostream& StructType::get_name(std::ostream &s) const {
	if (weak()) {
		s << '~';
	}
	return s << data->name;
}
std::ostream& StructType::c_name_raw(std::ostream &s) const {
	return s << "s_" << data->name;
}
std::ostream& StructType::c_declare_type(std::ostream &s) const {
	if (weak()) {
		return s;
	}

	s << "struct ";
	c_name_raw(s);
	return s << ";\n";
}
std::ostream& StructType::c_define_type(std::ostream &s) const override {
	if (weak()) {
		return s;
	}
	return s;
}

std::ostream& StructType::c_member_name(
	std::ostream &s,
	const std::string &name
) {
	return s << "m_" << name;
}

