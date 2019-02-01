#pragma once
#include "base-type-t.h"

class FlawedType : public TypeT {
public:
	virtual ~FlawedType();
	virtual bool assignable(const TypeT &a) const override;
	virtual bool is_declarable() const override;

	virtual void c_declare_type(std::ostream &s) const override;
	virtual void c_define_type(std::ostream &s) const override;
};
