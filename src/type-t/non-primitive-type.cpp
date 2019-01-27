#include "non-primitive-type.h"


const NonPrimitiveType* NonPrimitiveType::rvalue_conversion() const {
	return rvalueT;
}
bool NonPrimitiveType::assignable(const TypeT &a) const {
	const NonPrimitiveType *p = dynamic_cast<const NonPrimitiveType*>(&a);
	if (p == nullptr) {
		return false;
	}
	return rvalue_conversion() == p->rvalue_conversion();
}

