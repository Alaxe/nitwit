#pragma once

#include <string>
#include <memory>
#include <ostream>

class TypeT {
public:
	typedef std::unique_ptr<TypeT> UPtr;
	virtual ~TypeT();

	virtual bool assignable(const TypeT &a) const = 0;
	virtual bool is_lvalue() const = 0;
	virtual bool is_rvalue() const = 0;

	virtual std::ostream& get_name(std::ostream &s) const = 0;
	virtual std::string get_name() const;

	virtual std::ostream& c_declare_type(std::ostream &s) const = 0;
	virtual std::ostream& c_define_type(std::ostream &s) const = 0;

	virtual std::ostream& c_name(std::ostream &s) const = 0;
	virtual std::string c_name() const;
};
