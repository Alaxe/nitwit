#include "null-type.h"
#include "non-primitive-type.h"
#include <cassert>

bool NullType::assignable(const TypeT &a) const {
	return (
		(dynamic_cast<const NullType*> (&a))
		|| (dynamic_cast<const NonPrimitiveType*> (&a))
	);
}
void NullType::get_name(std::ostream &) const {
	assert(false);
}
void NullType::c_name(std::ostream &) const {
	assert(false);
}
