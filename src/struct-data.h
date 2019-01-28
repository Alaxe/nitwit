#pragma once

#include <string>
#include <unordered_map>

#include "lexer.h"
#include "type-t.h"
#include "context.h"

class StructData {
public:
	std::string name;
	std::unordered_map<std::string, const TypeT*> members;
};