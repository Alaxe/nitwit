#include "struct-type.h"

void StructType::c_struct_members(std::ostream &s) const {
	for (const auto &i : data.members) {
		s << "    ";
		i.second->c_name(s);
		s << " ";
		c_member_name(s, i.first);
		s << ";\n";
	}
}
void StructType::c_destroy_members(std::ostream &s) const {
	for (const auto &i : data.members) {
		const auto *mPtr =
			dynamic_cast<const NonPrimitiveType*> (i.second);
		if (mPtr == nullptr) {
			continue;
		}

		s << "    if(a->";
		c_member_name(s, i.first);
		s << ") {\n";

		s << "        ";
		mPtr->c_rm_ref_name(s);
		s << "(a->";
		c_member_name(s, i.first);
		s << ");\n";

		s << "    }\n";
	}
}
void StructType::c_define_alloc(std::ostream &s) const {
	c_name(s);
	s << " ";
	c_alloc_name(s);
	s << "(void) {\n";

	s << "    "; c_name(s); s << "a = ("; c_name(s); s << ") ";
	s << "calloc(1, sizeof(struct "; c_name_raw(s); s << "));\n";

	s << "    a->s_cnt = 1;\n";
	s << "    return a;\n";
	s << "}\n";
}
void StructType::c_define_extra_functions(std::ostream &s) const {
	for (const auto &i : data.members) {
		c_define_mget_l(s, i.first);
		c_define_mget_r(s, i.first);
	}
}
void StructType::c_define_mget_l(
	std::ostream &s,
	const std::string &member
) const {
	const TypeT *memT = get_member_type(member);

	memT->c_name(s);
	s << " ";
	c_member_access_name(s, member, true);
	c_standard_args(s);
	s << " {\n";

	s << "    return a->";
	c_member_name(s, member);
	s << ";\n";
	s << "}\n";
}
void StructType::c_define_mget_r(
	std::ostream &s,
	const std::string &member
) const {
	const TypeT *memT = get_member_type(member);

	memT->c_name(s);
	s << " ";
	c_member_access_name(s, member, false);
	c_standard_args(s);
	s << " {\n";

	s << "    "; memT->c_name(s); s << " m = a->";
	c_member_name(s, member); s << ";\n";
	if (dynamic_cast<const NonPrimitiveType*> (memT)) {
		s << "    m->s_cnt++;\n";
	}
	s << "    "; c_rm_ref_name(s); s << "(a);\n";
	s << "    return m;\n";
	s << "}\n";
}


StructType::StructType(const StructData &data): StructType(data, *this) {}
StructType::StructType(const StructData &data, const StructType &rvalueT):
	NonPrimitiveType(rvalueT), data(data) {}

const StructType& StructType::rvalue_conversion() const {
	return static_cast<const StructType&> (rvalueT);
}
const TypeT* StructType::get_member_type(const std::string& member) const {
	auto it = data.members.find(member);
	if (it == data.members.end()) {
		return nullptr;
	} else {
		return it->second;
	}
}

std::pair<StructType::UPtr, StructType::UPtr> StructType::make_pair(
	const StructData &data
) {
	StructType::UPtr strong(new StructType(data));
	StructType::UPtr weak(new StructType(data, *strong));

	return {std::move(strong), std::move(weak)};
}

void StructType::get_name(std::ostream &s) const {
	if (weak()) {
		s << '~';
	}
	s << data.name;
}
void StructType::c_name_raw(std::ostream &s) const {
	s << "s_" << data.name;
}
void StructType::c_member_access_name(
	std::ostream &s,
	const std::string &member,
	bool lvalue
) const {
	s << "mget_";
	s << (lvalue ? 'l' : 'r');
	s << "_" << std::to_string(member.size()) << "_";
	c_name_raw(s);
	s << "_" << member;
}

void StructType::c_member_name(std::ostream &s, const std::string &name) {
	s << "m_" << name;
}
