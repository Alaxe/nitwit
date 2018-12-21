#pragma once

#include <string>

class TypeT {
public:
	enum class Category {
		Void,
		Primitive,
		Reference,
		WReference
	};
	Category cat;
	std::string name;

	TypeT();
	TypeT(const Category &category, const std::string &name);
	std::string get_c_name() const;
};
