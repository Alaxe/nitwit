#pragma once

#include "non-primitive-type.h"

#include <utility>

class ArrayType : public NonPrimitiveType {
protected:

	void c_define_extra_functions(std::ostream &s) const override;
	void c_struct_members(std::ostream &s) const override;
	void c_destroy_members(std::ostream &s) const override;
	void c_define_alloc(std::ostream &s) const override;

	void c_index_args(std::ostream &s) const;
	void c_define_index_l(std::ostream &s) const;
	void c_define_index_r(std::ostream &s) const;

public:
	typedef std::unique_ptr<ArrayType> UPtr;

	const TypeT &elementT;

	ArrayType(const TypeT &elementT);
	ArrayType(const TypeT &elementT, const ArrayType &rvalueT);

	const ArrayType& rvalue_conversion() const override;
	static std::pair<UPtr, UPtr> make_pair(const TypeT &elementT);

	void c_index_name(std::ostream &s, bool lvalue) const;

	using TypeT::get_name;
	void get_name(std::ostream &s) const override;
	void c_name_raw(std::ostream &s) const override;
};
