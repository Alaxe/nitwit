#include "array-type.h"

const ArrayType* ArrayType::rvalue_conversion() const {
	return static_cast<const ArrayType*> (rvalueT);
}
