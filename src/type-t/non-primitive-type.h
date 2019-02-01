#pragma once

#include "base-type-t.h"

class NonPrimitiveType : public TypeT {
	//some helpers for C function generation
protected:
	const NonPrimitiveType &rvalueT;

public:
	NonPrimitiveType(const NonPrimitiveType &rvalueT);
	virtual ~NonPrimitiveType();
	bool weak() const;

	virtual const NonPrimitiveType& rvalue_conversion() const;

	virtual bool assignable(const TypeT &a) const override;
	virtual bool is_declarable() const override;
	virtual bool is_rvalue() const;

	virtual void c_name_raw(std::ostream &s) const = 0;
	using TypeT::c_name;
	virtual void c_name(std::ostream &s) const override;
};
