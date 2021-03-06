#pragma once
#include <memory>
#include <string>

#include "type-t.h"

class VarData {
public:
	typedef std::unique_ptr<VarData> UPtr;

	const TypeT &type;
	std::string name;
	VarData(const TypeT &type, std::string name);

	void c_name(std::ostream &out) const;
	void c_declaration(std::ostream &out, uint32_t indent = 0) const;
	void c_destruction(std::ostream &out, uint32_t indent = 0) const;

	static void c_generate_destructors(
		std::ostream &out,
		const std::vector<VarData> &vars,
		uint32_t indent = 0
	);
};
