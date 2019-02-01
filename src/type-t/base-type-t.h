#pragma once

#include <string>
#include <memory>
#include <ostream>

class TypeT {
public:
	typedef std::unique_ptr<TypeT> UPtr;
	virtual ~TypeT();

	virtual bool assignable(const TypeT &a) const = 0;
	virtual bool is_declarable() const = 0;

	virtual void get_name(std::ostream &s) const = 0;
	virtual std::string get_name() const;

	virtual void c_declare_type(std::ostream &s) const = 0;
	virtual void c_define_type(std::ostream &s) const = 0;

	virtual void c_name(std::ostream &s) const = 0;
	virtual std::string c_name() const;
};
