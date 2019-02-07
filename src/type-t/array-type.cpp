#include "array-type.h"

void ArrayType::c_define_extra_functions(std::ostream &s) const {
	c_define_index_l(s);
	c_define_index_r(s);
}
void ArrayType::c_struct_members(std::ostream &s) const {
	s << "    uint32_t size;\n";
	s << "    "; elementT.c_name(s); s << "* v;\n";
}
void ArrayType::c_destroy_members(std::ostream &s) const {
	const auto *elPtr = dynamic_cast<const NonPrimitiveType*>(&elementT);
	if (elPtr == nullptr) {
		return;
	}
	s << "    for (uint32_t i = 0;i < a->size;i++) {\n";
	s << "        if (a->v[i]) {\n";
	s << "            "; elPtr->c_rm_ref_name(s); s << "(a->v[i]);\n";
	s << "        }\n";
	s << "    }\n";
}
void ArrayType::c_define_alloc(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_alloc_name(s);
	s << "(uint32_t size) {\n";

	s << "    "; c_name(s); s << " a = ("; c_name(s); s << ") ";
	s << "calloc(1, sizeof(struct "; c_name_raw(s); s << "));\n";

	s << "    a->v = ("; elementT.c_name(s); s << "*)";
	s << "calloc(size, sizeof("; elementT.c_name(s); s << "));\n";

	s << "    a->size = size;\n";
	s << "    a->s_cnt = 1;\n";
	s << "    return a;\n";
	s << "}\n";
}


void ArrayType::c_index_args(std::ostream &s) const {
	s << "(";
	c_name(s);
	s << " a, uint32_t i)";
}
void ArrayType::c_define_index_l(std::ostream &s) const {
	elementT.c_name(s);
	s << " ";
	c_index_name(s, true);
	c_index_args(s);
	s << " {\n";

	s << "    return &a->v[i];\n";
	s << "}\n";
}
void ArrayType::c_define_index_r(std::ostream &s) const {
	elementT.c_name(s);
	s << " ";
	c_index_name(s, false);
	c_index_args(s);
	s << " {\n";

	s << "    "; elementT.c_name(s); s << " e = a->v[i];\n";

	const auto *elPtr = dynamic_cast<const NonPrimitiveType*>(&elementT);
	if (elPtr) {
		s << "    e->s_cnt++;\n";
	}

	s << "    "; c_rm_ref_name(s, false); s << "(a);\n";
	s << "    return e;\n";
	s << "}\n";
}

ArrayType::ArrayType(const TypeT &elementT): ArrayType(elementT, *this) {}
ArrayType::ArrayType(const TypeT &elementT, const ArrayType &rvalueT):
	NonPrimitiveType(rvalueT), elementT(elementT) {}

const ArrayType& ArrayType::rvalue_conversion() const {
	return static_cast<const ArrayType&> (rvalueT);
}

std::pair<ArrayType::UPtr, ArrayType::UPtr> ArrayType::make_pair(
	const TypeT &elementT
) {
	UPtr strong(new ArrayType(elementT));
	UPtr week(new ArrayType(elementT, *strong));
	return {std::move(strong), std::move(week)};
}

void ArrayType::c_index_name(std::ostream &s, bool lvalue) const {
	if (lvalue) {
		s << "*";
	}
	s << "index_";
	s << (lvalue ? 'l' : 'r') << '_';
	c_name_raw(s);
}

void ArrayType::get_name(std::ostream &s) const {
	if (weak()) {
		s << "~";
	}
	s << "#";
	elementT.get_name(s);
}
void ArrayType::c_name_raw(std::ostream &s) const {
	auto *t = dynamic_cast<const NonPrimitiveType*>(&elementT);

	s << "a";
	if ((t) && (t->weak())) {
		s << 'w';
	} else {
		s << 's';
	}
	s << "_";

	if (t) {
		t->c_name_raw(s);
	} else {
		elementT.c_name(s);
	}
}
