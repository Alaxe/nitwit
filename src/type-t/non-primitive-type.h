#pragma once

#include "base-type-t.h"

class NonPrimitiveType : public TypeT {
	//some helpers for C function generation
protected:
	const NonPrimitiveType &rvalueT;

	virtual void c_define_extra_functions(std::ostream &s) const;
	virtual void c_struct_members(std::ostream &s) const = 0;
	virtual void c_destroy_members(std::ostream &s) const = 0;
	virtual void c_define_alloc(std::ostream &s) const = 0;

	void c_assign_name(std::ostream &s, bool w, bool lvalue) const;
	void c_arg_pass_name(std::ostream &s, bool w, bool lvalue) const;
	void c_destroy_name(std::ostream &s) const;
	void c_rm_ref_name(std::ostream &s, bool w) const;

	void c_standard_args(std::ostream &s) const;
	void c_assign_args(std::ostream &s) const;

	void c_define_struct(std::ostream &s) const;
	void c_define_destroy(std::ostream &s) const;
	void c_define_norm_wval(std::ostream &s) const;

	void c_define_assign_sl(std::ostream &s) const;
	void c_define_assign_sr(std::ostream &s) const;
	void c_define_assign_wl(std::ostream &s) const;
	void c_define_assign_wr(std::ostream &s) const;

	void c_define_arg_pass_wl(std::ostream &s) const;
	void c_define_arg_pass_wr(std::ostream &s) const;
	void c_define_arg_pass_sl(std::ostream &s) const;

	void c_define_rm_sref(std::ostream &s) const;
	void c_define_rm_wref(std::ostream &s) const;


public:
	NonPrimitiveType(const NonPrimitiveType &rvalueT);
	virtual ~NonPrimitiveType();
	bool weak() const;

	virtual const NonPrimitiveType& rvalue_conversion() const;

	bool assignable(const TypeT &a) const override;
	bool is_declarable() const override;
	bool is_rvalue() const;

	virtual void c_name_raw(std::ostream &s) const = 0;
	using TypeT::c_name;
	void c_name(std::ostream &s) const override;
	void c_default_value(std::ostream &s) const override;

	void c_alloc_name(std::ostream &s) const;
	void c_assign_name(std::ostream &s, bool lvalue) const;
	void c_rm_ref_name(std::ostream &s) const;
	void c_norm_wval_name(std::ostream &s) const;
	void c_arg_pass_name(std::ostream &s, bool lvalue) const;

	void c_declare_type(std::ostream &s) const override;
	void c_define_type(std::ostream &s) const override;
	void c_define_helpers(std::ostream &s) const override;

};
