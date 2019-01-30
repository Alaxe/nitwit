#include "result-category.h"

ResultCategory::ResultCategory(uint32_t val): val(val) {}
ResultCategory::operator uint32_t () const {
	return val;
}
