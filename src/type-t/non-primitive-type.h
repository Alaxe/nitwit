#pragma once

#include "base-type-t.h"

class NonPrimitiveType : public TypeT {
	//some helpers for C function generation
protected:
	const NonPrimitiveType *rvalueT;

public:
	bool weak() const;

	virtual const NonPrimitiveType* rvalue_conversion() const;

	virtual bool assignable(const TypeT &a) const override;
	virtual bool is_lvalue() const override;
	virtual bool is_rvalue() const override;

	virtual std::ostream& c_name_raw(std::ostream &s) const = 0;
	virtual std::ostream& c_name(std::ostream &s) const override;
};
