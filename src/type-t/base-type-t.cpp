#include "base-type-t.h"

#include <sstream>
#include <cassert>

TypeT::~TypeT() {}
std::string TypeT::c_name() const {
	std::stringstream s;
	c_name(s);
	return s.str();
}

void TypeT::c_default_value(std::ostream &) const {
	assert(false);
}
std::string TypeT::get_name() const {
	std::stringstream s;
	get_name(s);
	return s.str();
}
