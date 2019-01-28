#pragma once

#include "base-type-t.h"

#include <vector>

class PrimitiveType : TypeT {
public:
	std::string name;
	bool isFloat;
	uint32_t castIndex;
	std::string cName;
	std::string scanfMacro;
	std::string printfMacro;

public:
	PrimitiveType(
		const std::string &name,
		bool isFloat,
		uint32_t castIndex,
		const std::string &cName,
		const std::string &scanfMacro,
		const std::string &printfMacro
	);

	const PrimitiveType& common_type(const PrimitiveType &t2) const;
	std::ostream& c_read_function(std::ostream &s) const;
	std::ostream& c_write_function(std::ostream &s) const;

	bool assignable(const TypeT &a) const override;
	bool is_lvalue() const override;
	bool is_rvalue() const override;
	std::ostream& get_name(std::ostream &s) const override;

	std::ostream& c_declare_type(std::ostream &s) const override;
	std::ostream& c_define_type(std::ostream &s) const override;
	std::ostream& c_name(std::ostream &s) const override;

	
	static const std::string maxInt;
	static const std::string maxFloat;
	static const std::string defaultInt; 

	static const std::vector<PrimitiveType> types;
};
