#include "flawed-type.h"

FlawedType::~FlawedType() {}
bool FlawedType::assignable(const TypeT &) const {
	return false;
}
bool FlawedType::is_declarable() const {
	return false;
}
	
void FlawedType::c_declare_type(std::ostream &) const {}
void FlawedType::c_define_type(std::ostream &) const {}
void FlawedType::c_define_helpers(std::ostream &) const {}
