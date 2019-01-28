#include "base-type-t.h"

#include <sstream>

TypeT::~TypeT() {}
std::string TypeT::c_name() const {
	std::stringstream s;
	c_name(s);
	return s.str();
}

std::string TypeT::get_name() const {
	std::stringstream s;
	name(s);
	return s.str();
}
