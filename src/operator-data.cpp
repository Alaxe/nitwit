#include "operator-data.h"

std::unordered_map<std::string, OperatorData> OperatorData::operators;

OperatorData::OperatorData(const std::string &name, uint32_t arity,
	bool takeFloat, const std::string &cName):
	name(name), arity(arity), takeFloat(takeFloat), cName(cName) {}


void OperatorData::init_if_needed() {
	if (!operators.empty()) {
		return;
	}
	operators.emplace("+", OperatorData("+", 2, true, "+"));
	operators.emplace("-", OperatorData("-", 2, true, "-"));
	operators.emplace("*", OperatorData("*", 2, true, "*"));
	operators.emplace("/", OperatorData("/", 2, true, "/"));
	operators.emplace("\\\\", OperatorData("\\\\", 2, false, "%"));
	
	operators.emplace("==", OperatorData("==", 2, true, "=="));
	operators.emplace("!=", OperatorData("!=", 2, true, "!="));
	operators.emplace(">=", OperatorData(">=", 2, true, ">="));
	operators.emplace("<=", OperatorData("<=", 2, true, "<="));
	operators.emplace(">", OperatorData(">", 2, true, ">"));
	operators.emplace("<", OperatorData("<", 2, true, "<"));

	operators.emplace("&", OperatorData("&", 2, false, "&&"));
	operators.emplace("|", OperatorData("|", 2, false, "||"));
	operators.emplace("~", OperatorData("~", 1, false, "!"));
}
const OperatorData* OperatorData::get(const std::string &name) {
	init_if_needed();

	auto it = operators.find(name);
	if (it != operators.end()) {
		return &it->second; //fine as once the map is constructed, it isn't modified
	} else {
		return nullptr;
	}
}
