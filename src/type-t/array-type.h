#pragma once

#include "non-primitive-type.h"

#include <utility>

class ArrayType : public NonPrimitiveType {
public:
	typedef std::unique_ptr<ArrayType> UPtr;

	const TypeT &elementT;

	ArrayType(const TypeT &elementT);
	ArrayType(const TypeT &elementT, const ArrayType &rvalueT);
	virtual ~ArrayType();

	virtual const ArrayType& rvalue_conversion() const override;
	static std::pair<UPtr, UPtr> make_pair(const TypeT &elementT);

	using TypeT::get_name;
	virtual void get_name(std::ostream &s) const override;
	virtual void c_declare_type(std::ostream &s) const override;
	virtual void c_define_type(std::ostream &s) const override;
	virtual void c_name_raw(std::ostream &s) const override;
};
