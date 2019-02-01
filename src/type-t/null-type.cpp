#include "null-type.h"
#include <cassert>

NullType::~NullType() {}
void NullType::get_name(std::ostream &) const {
	assert(false);
}
void NullType::c_name(std::ostream &) const {
	assert(false);
}
