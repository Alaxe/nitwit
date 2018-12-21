#pragma once

#include <string>
#include <unordered_map>

class PrimTypeData {
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

	static std::unordered_map<std::string, PrimTypeData> types;
	static void init_if_needed();

public:
	static const PrimTypeData* get(const std::string &name);
	static const PrimTypeData* combine(
		const PrimTypeData *t1,
		const PrimTypeData *t2
	);
	static std::string get_max_int();
	static std::string get_max_float();
	static const std::unordered_map<std::string, PrimTypeData>& get_all();
};
