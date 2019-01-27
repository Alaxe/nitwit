#pragma once

#include "non-primitive-type.h"

class StructType : public NonPrimitiveType {
public:
	const StructData *data;

	const StructType* rvalue_conversion() const;
};
