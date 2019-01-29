#pragma once
#include <memory>
#include <ostream>

#include "base-type-t.h"

class VoidType : public TypeT {
public:
	typedef std::unique_ptr<VoidType> UPtr;

	virtual bool assignable(const TypeT &a) const;
	virtual bool is_lvalue() const;
	virtual bool is_rvalue() const;

	virtual std::ostream& get_name(std::ostream &s) const;

	virtual std::ostream& c_declare_type(std::ostream &s) const;
	virtual std::ostream& c_define_type(std::ostream &s) const;

	virtual std::ostream& c_name(std::ostream &s) const;
};
