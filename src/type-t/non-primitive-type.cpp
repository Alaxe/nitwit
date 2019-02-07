#include "non-primitive-type.h"

void NonPrimitiveType::c_assign_name(
	std::ostream &s,
	bool isWeak,
	bool lvalue
) const {
	s << "*assign_";
	s << (isWeak ? 'w' : 's');
	s << (lvalue ? 'l' : 'r');
	s << '_';
	c_name_raw(s);
}
void NonPrimitiveType::c_arg_pass_name(
	std::ostream &s,
	bool isWeak,
	bool lvalue
) const {
	if ((!isWeak) && (!lvalue)) {
		return;
	}
	s << "arg_";
	s << (isWeak ? 'w' : 's');
	s << (lvalue ? 'l' : 'r');
	s << "_";
	c_name_raw(s);
}
void NonPrimitiveType::c_destroy_name(std::ostream &s) const {
	s << "destroy_";
	c_name_raw(s);
}
void NonPrimitiveType::c_rm_ref_name(std::ostream &s, bool isWeak) const {
	s << "rm_";
	s << (isWeak ? 'w' : 's');
	s << "_ref_";
	c_name_raw(s);
}

void NonPrimitiveType::c_standard_args(std::ostream &s) const {
	s << "(";
	c_name(s);
	s << " a)";
}
void NonPrimitiveType::c_assign_args(std::ostream &s) const {
	s << "(";
	c_name(s);
	s << "* a, ";
	c_name(s);
	s << " b)";
}

void NonPrimitiveType::c_define_extra_functions(std::ostream &) const {}

void NonPrimitiveType::c_define_struct(std::ostream &s) const {
	s << "struct ";
	c_name_raw(s);

	s << " {\n";
	s << "    uint32_t s_cnt;\n";
	s << "    uint32_t w_cnt;\n";
	c_struct_members(s);
	s << "};\n";
}
void NonPrimitiveType::c_define_destroy(std::ostream &s) const {
	s << "void ";
	c_destroy_name(s);
	s << "(";
	c_name(s);
	s << " a) {\n";

	c_destroy_members(s);
	s <<
	"    if (!a->w_cnt) {\n"
	"        free(a);\n"
	"    }\n"
	"}\n";
}
void NonPrimitiveType::c_define_norm_wval(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_norm_wval_name(s);
	c_standard_args(s);
	s << " {\n";
	
	s << "    if ((a) && (a->w_cnt == 0)) {\n";
	s << "        return NULL;\n";
	s << "    } else {\n";
	s << "        return a;\n";
	s << "    }\n";
	s << "}\n";
}

void NonPrimitiveType::c_define_assign_sl(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_assign_name(s, false, true);
	c_assign_args(s);
	s << " {\n";
	
	s << "    if (b) {\n";
	s << "        b->s_cnt++;\n";
	s << "    }\n";
	s << "    if (*a) {\n";
	s << "        "; c_rm_ref_name(s, false); s << "(*a);\n";
	s << "    }\n";
	s << "    *a = b;\n";
	s << "    return a;\n";
	s << "}\n";
}
void NonPrimitiveType::c_define_assign_sr(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_assign_name(s, false, false);
	c_assign_args(s);
	s << " {\n";

	s << "    if (*a) {\n";
	s << "        "; c_rm_ref_name(s, false); s << "(*a);\n";
	s << "    }\n";
	s << "    *a = b;\n";
	s << "    return a;\n";
        s << "}\n";
}
void NonPrimitiveType::c_define_assign_wl(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_assign_name(s, true, true);
	c_assign_args(s);
	s << " {\n";

	s << "    if (b) {\n";
	s << "        b->w_cnt++;\n";
	s << "    }\n";
	s << "    if (*a) {\n";
	s << "        "; c_rm_ref_name(s, true) ;s << "(*a);\n";
	s << "    }\n";
	s << "    *a = b;\n";
	s << "    return a;\n";
	s << "}\n";
}
void NonPrimitiveType::c_define_assign_wr(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_assign_name(s, true, false);
	c_assign_args(s);
	s << " {\n";
	
	s << "    if (b) {\n";
	s << "        b->w_cnt++;\n";
	s << "        "; c_rm_ref_name(s, false); s << "(b);\n";
	s << "    }\n";
	s << "    if (*a) {\n";
	s << "        "; c_rm_ref_name(s, true); s << "(*a);\n";
	s << "    }\n";
	s << "    *a = b;\n";
	s << "    return a;\n";
	s << "}\n";
}
void NonPrimitiveType::c_define_arg_pass_wl(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_arg_pass_name(s, true, true);
	c_standard_args(s);
	s << " {\n";
	s << "    if (a) {\n";
	s << "        a->w_cnt++;\n";
	s << "    }\n";
	s << "    return a;\n";
	s << "}\n";
}
void NonPrimitiveType::c_define_arg_pass_wr(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_arg_pass_name(s, true, false);
	c_standard_args(s);
	s << " {\n";
	s << "    if (a) {\n";
	s << "        a->w_cnt++;\n";
	s << "        "; c_rm_ref_name(s, false); s << "(a);\n";
	s << "    }\n";
	s << "    return a;\n";
	s << "}\n";
}
void NonPrimitiveType::c_define_arg_pass_sl(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_arg_pass_name(s, false, true);
	c_standard_args(s);
	s << " {\n";
	s << "    if (a) {\n";
	s << "        a->s_cnt++;\n";
	s << "    }\n";
	s << "    return a;\n";
	s << "}\n";
}

void NonPrimitiveType::c_define_rm_sref(std::ostream &s) const {
	s << "void ";
	c_rm_ref_name(s, false);
	c_standard_args(s);
	s << " {\n";

	s << "    a->s_cnt--;\n";
	s << "    if (!a->s_cnt) {\n";
	s << "        "; c_destroy_name(s); s << "(a);\n";
	s << "    }\n";
	s << "}\n";
}
void NonPrimitiveType::c_define_rm_wref(std::ostream &s) const {
	s << "void ";
	c_rm_ref_name(s, true);
	c_standard_args(s);
	s << " {\n";

	s << "    a->w_cnt--;\n";
	s << "    if (!(a->w_cnt | a->s_cnt)) {\n";
	s << "        free(a);\n";
	s << "    }\n";
	s << "}\n";
}


NonPrimitiveType::NonPrimitiveType(const NonPrimitiveType &rvalueT):
	rvalueT(rvalueT) {}

NonPrimitiveType::~NonPrimitiveType() {}
const NonPrimitiveType& NonPrimitiveType::rvalue_conversion() const {
	return rvalueT;
}
bool NonPrimitiveType::weak() const {
	return &rvalueT != this;
}

bool NonPrimitiveType::assignable(const TypeT &a) const {
	const NonPrimitiveType *p = dynamic_cast<const NonPrimitiveType*>(&a);
	if (p == nullptr) {
		return false;
	}
	return &rvalue_conversion() == &p->rvalue_conversion();
}
bool NonPrimitiveType::is_declarable() const {
	return true;
}
bool NonPrimitiveType::is_rvalue() const {
	return !weak();
}

void NonPrimitiveType::c_name(std::ostream &s) const {
	s << "struct ";
	c_name_raw(s);
	s << '*';
}
void NonPrimitiveType::c_alloc_name(std::ostream &s) const {
	s << "alloc_";
	c_name_raw(s);
}
void NonPrimitiveType::c_assign_name(std::ostream &s, bool lvalue) const {
	c_assign_name(s, weak(), lvalue);
}
void NonPrimitiveType::c_arg_pass_name(std::ostream &s, bool lvalue) const {
	c_arg_pass_name(s, weak(), lvalue);
}

void NonPrimitiveType::c_rm_ref_name(std::ostream &s) const {
	c_rm_ref_name(s, weak());
}
void NonPrimitiveType::c_norm_wval_name(std::ostream &s) const {
	s << "wval_";
	c_name_raw(s);
}

void NonPrimitiveType::c_declare_type(std::ostream &s) const {
	if (weak()) {
		return;
	}
	s << "struct ";
	c_name_raw(s);
	s << ";\n";
}
void NonPrimitiveType::c_define_type(std::ostream &s) const {
	if (weak()) {
		return;
	}
	c_define_struct(s);

	s << "void ";
	c_rm_ref_name(s, true);
	c_standard_args(s);
	s << ";\n";

	s << "void ";
	c_rm_ref_name(s, false);
	c_standard_args(s);
	s << ";\n";
}
void NonPrimitiveType::c_define_helpers(std::ostream &s) const {
	if (weak()) {
		return;
	}
	c_define_destroy(s);
	c_define_rm_sref(s);
	c_define_rm_wref(s);

	c_define_alloc(s);

	c_define_assign_sl(s);
	c_define_assign_sr(s);
	c_define_assign_wl(s);
	c_define_assign_wr(s);

	c_define_arg_pass_sl(s);
	c_define_arg_pass_wl(s);
	c_define_arg_pass_wr(s);

	c_define_extra_functions(s);
}
