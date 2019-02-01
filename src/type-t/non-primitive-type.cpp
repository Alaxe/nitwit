#include "non-primitive-type.h"


NonPrimitiveType::NonPrimitiveType(const NonPrimitiveType &rvalueT):
	rvalueT(rvalueT) {}

NonPrimitiveType::~NonPrimitiveType() {}
const NonPrimitiveType& NonPrimitiveType::rvalue_conversion() const {
	return rvalueT;
}
bool NonPrimitiveType::weak() const {
	return &rvalueT != this;
}

bool NonPrimitiveType::assignable(const TypeT &a) const {
	const NonPrimitiveType *p = dynamic_cast<const NonPrimitiveType*>(&a);
	if (p == nullptr) {
		return false;
	}
	return &rvalue_conversion() == &p->rvalue_conversion();
}
bool NonPrimitiveType::is_declarable() const {
	return true;
}
bool NonPrimitiveType::is_rvalue() const {
	return !weak();
}

void NonPrimitiveType::c_name(std::ostream &s) const {
	c_name_raw(s);
	s << '*';
}
