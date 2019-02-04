#pragma once

#include "base-type-t.h"

#include <vector>

class PrimitiveType : public TypeT {
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
		std::string cName,
		std::string scanfMacro,
		std::string printfMacro
	);

	const PrimitiveType& common_type(const PrimitiveType &t2) const;
	void c_read_function(std::ostream &s) const;
	void c_write_function(std::ostream &s) const;

	bool assignable(const TypeT &a) const override;
	bool is_declarable() const override;

	using TypeT::get_name;
	using TypeT::c_name;

	void get_name(std::ostream &s) const override;
	void c_declare_type(std::ostream &s) const override;
	void c_define_type(std::ostream &s) const override;
	void c_name(std::ostream &s) const override;

	static const std::string maxInt;
	static const std::string maxFloat;
	static const std::string defaultInt;

	static const std::vector<PrimitiveType> types;
};
