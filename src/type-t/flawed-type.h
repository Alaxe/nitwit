#pragma once
#include "base-type-t.h"

class FlawedType : public TypeT {
public:
	virtual ~FlawedType();
	bool assignable(const TypeT &a) const override;
	bool is_declarable() const override;

	void c_declare_type(std::ostream &s) const override;
	void c_define_type(std::ostream &s) const override;
	void c_define_helpers(std::ostream &s) const override;
};
