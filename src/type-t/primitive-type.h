#pragma once

#include "base-type-t.h"

#include <string>

class PrimitiveType : TypeT {
public:
	std::string name;
	bool isFloat;
	uint32_t castIndex;
	std::string cName;
	std::string scanfMacro;
	std::string printfMacro;

private:
	PrimTypeData(
		const std::string &name,
		bool isFloat,
		uint32_t castIndex,
		const std::string &cName,
		const std::string &scanfMacro,
		const std::string &printfMacro
	);


public:
	const PrimTypeData* combine(const PrimTypeData *t2) const;
	static const PrimitiveType* get_max_int(const GlobalContext &gc);
	static const PrimitiveType* get_max_float();
};
