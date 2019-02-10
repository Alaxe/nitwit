#include "operator-data.h"


OperatorData::OperatorData(
	TokenType type,
	bool takeFloat,
	bool resCommon,
	const std::string &cInfix
): type(type), takeFloat(takeFloat), resCommon(resCommon), cInfix(cInfix) {}

OperatorData::OperatorData(
	TokenType type,
	bool takeFloat,
	bool resCommon,
	const std::string &cPrefix,
	const std::string &cInfix,
	const std::string &cSuffix
): type(type), takeFloat(takeFloat), resCommon(resCommon), cPrefix(cPrefix),
	cInfix(cInfix), cSuffix(cSuffix) {}

std::unordered_map<TokenType, OperatorData> OperatorData::operators = {
	{TokenType::Plus, {TokenType::Plus, true, true, "+"}},
	{TokenType::Minus, {TokenType::Minus, true, true, "-"}},
	{TokenType::Times, {TokenType::Times, true, true, "*"}},
	{TokenType::Divide, {TokenType::Divide, true, true, "/"}},
	{TokenType::Remainder, {TokenType::Remainder, false, true, "%"}},

	{TokenType::Less, {TokenType::Less, true, false, "<"}},
	{TokenType::Greater, {TokenType::Greater, true, false, ">"}},
	{TokenType::LessEq, {TokenType::LessEq, false, false, "<="}},
	{TokenType::GreatEq, {TokenType::GreatEq, false, false, ">="}},

	{TokenType::And, {TokenType::And, false, false, "&"}},
	{TokenType::Or, {TokenType::Or, false, false, "|"}},
	{TokenType::Xor, {TokenType::Xor, false, false, "!", "!= !", ""}},
	{TokenType::Not, {TokenType::Not, false, false, "~"}},

	{TokenType::AugPlus, {TokenType::AugPlus, true, true, "+="}},
	{TokenType::AugMinus, {TokenType::AugMinus, true, true, "-="}},
	{TokenType::AugTimes, {TokenType::AugTimes, true, true, "*="}},
	{TokenType::AugDivide, {TokenType::AugDivide, true, true, "/="}},
	{TokenType::AugRem, {TokenType::AugRem, false, true, "%="}},
};

const OperatorData* OperatorData::get(const TokenType &t) {
	auto it = operators.find(t);
	if (it != operators.end()) {
		return &it->second;
	} else {
		return nullptr;
	}
}
