#include "array-type.h"

ArrayType::ArrayType(const TypeT &elementT): ArrayType(elementT, *this) {}
ArrayType::ArrayType(const TypeT &elementT, const ArrayType &rvalueT):
	NonPrimitiveType(rvalueT), elementT(elementT) {}
ArrayType::~ArrayType() {}

const ArrayType& ArrayType::rvalue_conversion() const {
	return static_cast<const ArrayType&> (rvalueT);
}

std::pair<ArrayType::UPtr, ArrayType::UPtr> ArrayType::make_pair(
	const TypeT &elementT
) {
	UPtr strong(new ArrayType(elementT));
	UPtr week(new ArrayType(elementT, *strong));
	return {std::move(strong), std::move(week)};
}
std::ostream& ArrayType::get_name(std::ostream &s) const {
	if (weak()) {
		s << "~";
	}
	s << "#";
	return elementT.get_name(s);
}
std::ostream& ArrayType::c_declare_type(std::ostream &s) const {
	if (weak()) {
		return s;
	}
	s << "struct ";
	c_name_raw(s);
	return s << ";\n";
}
std::ostream& ArrayType::c_define_type(std::ostream &s) const {
	if (weak()) {
		return s;
	}
	return s;
}
std::ostream& ArrayType::c_name_raw(std::ostream &s) const {
	s << "a";
	s << (weak() ? 'w' : 's');
	s << "_";
	auto *t = dynamic_cast<const NonPrimitiveType*>(&elementT);
	if (t) {
		return t->c_name_raw(s);
	} else {
		return elementT.c_name(s);
	}
}
