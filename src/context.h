#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include "type-t.h"
#include "function-t.h"
#include "lexer.h"

class Context {
private:
	std::unordered_map<std::string, FunctionT> functions;
	std::unordered_map<std::string, TypeT> globalVar;
	std::unordered_map<std::string, TypeT> localVar;

public:
	void declare_function(const std::string &name, const FunctionT &proto);
	void declare_global_var(const std::string &name, const TypeT &type);
	void declare_local_var(const std::string &name, const TypeT &type);
	void remove_locals();

	const FunctionT* get_function(const std::string &name) const;
	const TypeT* get_variable(const std::string &name) const;

	static Context generate_global(const std::vector<Line> &lines);
};
