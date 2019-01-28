#pragma once

#include "non-primitive-type.h"
#include "../struct-data.h"

class StructType : public NonPrimitiveType {
public:
	const StructData *data;

	const StructType* rvalue_conversion() const;
};
