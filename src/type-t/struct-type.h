#pragma once

#include <memory>
#include <utility>

#include "non-primitive-type.h"
#include "../struct-data.h"

class StructType : public NonPrimitiveType {
protected:
	void c_struct_members(std::ostream &s) const override;
	void c_destroy_members(std::ostream &s) const override;
	void c_define_alloc(std::ostream &s) const override;
	void c_define_extra_functions(std::ostream &s) const override;

	void c_define_mget_l(std::ostream &s, const std::string &member) const;
	void c_define_mget_r(std::ostream &s, const std::string &member) const;

public:
	typedef std::unique_ptr<StructType> UPtr;
	const StructData &data;


	StructType(const StructData &data);
	StructType(const StructData &data, const StructType &rvalueT);

	const StructType& rvalue_conversion() const override;
	const TypeT* get_member_type(const std::string& memberName) const;

	static std::pair<UPtr, UPtr> make_pair(const StructData &data);

	using TypeT::get_name;
	void get_name(std::ostream &s) const override;
	void c_name_raw(std::ostream &s) const override;
	void c_member_access_name(
		std::ostream &s,
		const std::string &member,
		bool lvalue
	) const;


	static void c_member_name(std::ostream &s, const std::string &name);
};
