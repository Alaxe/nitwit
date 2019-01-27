#pragma once

class TypeT {
public:
	virtual ~TypeT();
	virtual bool assignable(const TypeT &a) const = 0;
};
