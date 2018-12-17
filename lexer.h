#pragma once
#include <cstdint>
#include <istream>
#include <vector>
#include <string>

enum class TokenType {
	Unknown,
	Operator,
	DefVar,
	DefWeak,
	DefFunc,
	Return,
	LitInt,
	Identifier,
};

class Token {
public:
	TokenType type;
	uint32_t charInd;
	std::string s;

	Token(uint32_t charInd = 0);
};

class Line {
public:
	std::string s;

	uint32_t lineInd;
	uint32_t indent;
	std::vector<Token> tokens;

	Line(uint32_t lineInd = 0);
};

std::vector<Line> split_lines(std::istream &in);
std::vector<Token> tokenize(const std::string &s);
