#include "prim-type-data.h"

PrimTypeData::PrimTypeData(
	const std::string &name,
	bool isFloat,
	uint32_t castIndex,
	const std::string &cName
): name(name),	isFloat(isFloat), castIndex(castIndex), cName(cName) {}

std::unordered_map<std::string, PrimTypeData> PrimTypeData::types;

void PrimTypeData::init_if_needed() {
	if (!types.empty()) {
		return;
	}
	types.emplace("real", PrimTypeData("real", true, 0, "double"));
	types.emplace("float", PrimTypeData("float", true, 1, "float"));

	types.emplace("uint64", PrimTypeData("uint64", false, 2, "uint64_t"));
	types.emplace("int64", PrimTypeData("int64", false, 3, "int64_t"));

	types.emplace("uint32", PrimTypeData("uint32", false, 4, "uint32_t"));
	types.emplace("uint", PrimTypeData("uint", false, 5, "uint32_t"));
	types.emplace("int32", PrimTypeData("int32", false, 6, "int32_t"));
	types.emplace("int", PrimTypeData("int", false, 7, "int32_t"));

	types.emplace("uint16", PrimTypeData("uint16", false, 8, "uint16_t"));
	types.emplace("int16", PrimTypeData("int16", false, 9, "int16_t"));

	types.emplace("uint8", PrimTypeData("uint8", false, 10, "uint8_t"));
	types.emplace("int8", PrimTypeData("int8", false, 11, "int8_t"));
}

const PrimTypeData* PrimTypeData::get(const std::string &name) {
	init_if_needed();
	auto it = types.find(name);
	if (it == types.end()) {
		return nullptr;
	} else {
		return &it->second;
	}
}

const PrimTypeData* PrimTypeData::combine(
	const PrimTypeData *t1,
	const PrimTypeData *t2
) {
	if ((t1 == nullptr) || (t2 == nullptr)) {
		return nullptr;
	} else {
		return t2->castIndex < t1->castIndex ? t2 : t1;
	}
}

std::string PrimTypeData::get_max_int() {
	return "int64";
}
std::string PrimTypeData::get_max_float() {
	return "real";
}
