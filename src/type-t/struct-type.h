#pragma once

#include <memory>
#include <utility>

#include "non-primitive-type.h"
#include "../struct-data.h"

class StructType : public NonPrimitiveType {
public:
	typedef std::unique_ptr<StructType> UPtr;
	const StructData &data;


	StructType(const StructData &data);
	StructType(const StructData &data, const StructType &rvalueT);

	virtual const StructType& rvalue_conversion() const override;
	static std::pair<UPtr, UPtr> make_pair(const StructData &data);

	using TypeT::get_name;
	std::ostream& get_name(std::ostream &s) const override;
	std::ostream& c_declare_type(std::ostream &s) const override;
	std::ostream& c_define_type(std::ostream &s) const override;
	std::ostream& c_name_raw(std::ostream &s) const override;

	static std::ostream& c_member_name(
		std::ostream &s,
		const std::string &name
	);
};
