#pragma once

#include "non-primitive-type.h"

class ArrayType : public NonPrimitiveType {
private:
	const TypeT *elementT;
	const ArrayType* rvalue_conversion() const;
};
