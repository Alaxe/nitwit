#pragma once

#include "base-type-t.h"

class NonPrimitiveType : public TypeT {
	//some helpers for C function generation
protected:
	bool week;
	const NonPrimitiveType *rvalueT;

	const NonPrimitiveType* rvalue_conversion() const;
	virtual bool assignable(const TypeT &a) const;
};
