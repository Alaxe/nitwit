#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

class OperatorData {
public:
	std::string name;
	uint32_t arity;
	bool takeFloat;
	std::string cName;

private:
	static std::unordered_map<std::string, OperatorData> operators;

	OperatorData(const std::string &name, uint32_t arity, bool takeFloat,
		const std::string &cName);

	static void init_if_needed();
public:
	static const OperatorData* get(const std::string &name);
};
