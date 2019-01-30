#pragma once
#include <cstdint>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

class TokenType {
public:
	enum : uint32_t {
		Literal    = 0x800000,
		Identifier = 0x400000,
		KeySeq     = 0x200000,
		Operator   = 0x100000,

		BinOpr     = 0x080000,
		UnOpr      = 0x040000,
		AugOpr     = 0x020000,

		Unknown    = 0x000000,

		LitInt     = 0x800001,
		LitFloat   = 0x800002,
		LitNull    = 0x800003,

		VarDef     = 0x200001,
		FuncDef    = 0x200002,
		StructDef  = 0x200003,
		Return     = 0x200004,
		If         = 0x200005,
		Elif       = 0x200006,
		Else       = 0x200007,
		While      = 0x200008,
		Break      = 0x200009,
		Continue   = 0x20000A,

		Assign     = 0x300001,
		Equal      = 0x300002,
		NotEqual   = 0x300003,
		Member     = 0x300004,
		Index      = 0x300005,
		Alloc      = 0x300006,
		ArrSize    = 0x300007,
		FuncCall   = 0x300008,
		Read       = 0x300009,
		Write      = 0x30000A,

		Plus       = 0x380001,
		Minus      = 0x380002,
		Times      = 0x380003,
		Divide     = 0x380004,
		Remainder  = 0x380005,
		Less       = 0x380006,
		Greater    = 0x380007,
		LessEq     = 0x380008,
		GreatEq    = 0x380009,
		And        = 0x38000A,
		Or         = 0x38000B,
		Xor        = 0x38000C,

		Not        = 0x340001
	};
private:
	static std::unordered_map<std::string, uint32_t> tokStrings;
public:
	uint32_t val;
	TokenType(uint32_t val = 0);

	static TokenType type_from_string(const std::string &s);
	operator uint32_t () const;
};
namespace std {
	template <>
	struct hash<TokenType> {
		std::size_t operator()(const TokenType& t) const;
	};
}

class Token {
public:
	typedef std::vector<Token>::iterator It;
	typedef std::vector<Token>::const_iterator ConstIt;

	TokenType type;
	uint32_t startInd, endInd;
	std::string s;

	Token(uint32_t startInd = 0);

	static std::vector<Token> tokenize(const std::string &s);
};
std::ostream& operator<< (std::ostream &out, const Token &t);

class Line {
public:
	typedef std::vector<Line>::iterator It;
	typedef std::vector<Line>::const_iterator ConstIt;

	std::string s;

	uint32_t lineInd;
	uint32_t indent;
	std::vector<Token> tokens;

	Line(uint32_t lineInd = 0);

	static std::vector<Line> split_stream(std::istream &in);
};
std::ostream& operator<< (std::ostream &out, const Line &l);
