#pragma once
#include "flawed-type.h"

class NullType : public FlawedType {
public:
	typedef std::unique_ptr<NullType> UPtr;

	bool assignable(const TypeT &a) const override;
	using TypeT::get_name;
	virtual void get_name(std::ostream &s) const override;
	using TypeT::c_name;
	virtual void c_name(std::ostream &s) const override;
};
