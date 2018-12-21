#include "type-t.h"

#include <cassert>

#include "prim-type-data.h"

TypeT::TypeT(): cat(Category::Void) {}
TypeT::TypeT(const Category &cat, const std::string &name):
	cat(cat), name(name) {}

std::string TypeT::get_c_name() const {
	if (cat == Category::Void) {
		return "void";
	} else if (cat == Category::Primitive) {
		const auto *p = PrimTypeData::get(name);
		assert(p);
		return p->cName;
	} else {
		assert(false);
	}
}
