#include "void-type.h"

bool VoidType::assignable(const TypeT &a) const {
	return false;
}
bool VoidType::is_lvalue() const {
	return false;
}
bool VoidType::is_rvalue() const {
	return false;
}

std::ostream& VoidType::get_name(std::ostream &s) const {
	return s << "void";
}

std::ostream& VoidType::c_declare_type(std::ostream &s) const {}
std::ostream& VoidType::c_define_type(std::ostream &s) const {}

std::ostream& VoidType::c_name(std::ostream &s) const {
	return s << "void";
}

