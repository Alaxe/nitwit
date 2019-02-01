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
void ArrayType::get_name(std::ostream &s) const {
	if (weak()) {
		s << "~";
	}
	s << "#";
	elementT.get_name(s);
}
void ArrayType::c_declare_type(std::ostream &s) const {
	if (weak()) {
		return;
	}
	s << "struct ";
	c_name_raw(s);
	s << ";\n";
}
void ArrayType::c_define_type(std::ostream &s) const {
	if (weak()) {
		return;
	}
}
void ArrayType::c_name_raw(std::ostream &s) const {
	s << "a";
	s << (weak() ? 'w' : 's');
	s << "_";
	auto *t = dynamic_cast<const NonPrimitiveType*>(&elementT);
	if (t) {
		t->c_name_raw(s);
	} else {
		elementT.c_name(s);
	}
}
