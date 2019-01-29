#include "non-primitive-type.h"


const NonPrimitiveType* NonPrimitiveType::rvalue_conversion() const {
	return rvalueT;
}
bool NonPrimitiveType::weak() const {
	return rvalueT != this;
}

bool NonPrimitiveType::assignable(const TypeT &a) const {
	const NonPrimitiveType *p = dynamic_cast<const NonPrimitiveType*>(&a);
	if (p == nullptr) {
		return false;
	}
	return rvalue_conversion() == p->rvalue_conversion();
}
bool NonPrimitiveType::is_lvalue() const {
	return true;
}
bool NonPrimitiveType::is_rvalue() const {
	return !weak();
}

std::ostream& NonPrimitiveType::c_name(std::ostream &s) const {
	c_name_raw(s);
	return s << '*';
}
