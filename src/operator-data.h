#pragma once
#include <cstdint>
#include <string>
#include <unordered_map>

#include "lexer.h"

class OperatorData {
public:
	TokenType type;
	bool takeFloat;
	bool resCommon;

	std::string cPrefix;
	std::string cInfix;
	std::string cSuffix;

private:

	OperatorData(
		TokenType type,
		bool takeFloat,
		bool resCommon,
		const std::string &cInfix
	);
	OperatorData(
		TokenType type,
		bool takeFloat,
		bool resCommon,
		const std::string &cPrefix,
		const std::string &cInfix,
		const std::string &CSuffix
	);

	static std::unordered_map<TokenType, OperatorData> operators;
public:
	static const OperatorData* get(const TokenType &t);
};
