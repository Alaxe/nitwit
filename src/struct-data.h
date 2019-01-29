#pragma once

#include <string>
#include <unordered_map>

#include "lexer.h"
#include "type-t.h"

class StructData {
public:
	typedef std::unique_ptr<StructData> UPtr;

	std::string name;
	std::unordered_map<std::string, const TypeT*> members;
};
