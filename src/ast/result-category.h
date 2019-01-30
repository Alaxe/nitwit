#pragma once
#include <cstdint>

class ResultCategory {
public:
	enum : uint32_t {
		Value      = 0x8000,
		Unknown    = 0x0000,
		RValue     = 0x8001,
		LValue     = 0x8002,
		Identifier = 0x0001
	};
	uint32_t val;
	ResultCategory(uint32_t val = 0);

	operator uint32_t () const;
};
