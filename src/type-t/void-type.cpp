#include "void-type.h"

VoidType::~VoidType() {}
void VoidType::get_name(std::ostream &s) const {
	s << "void";
}
void VoidType::c_name(std::ostream &s) const {
	s << "void";
}
