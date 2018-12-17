#include "lexer.h"

Token::Token(uint32_t charInd): type(TokenType::Unknown), charInd(charInd) {}

Line::Line(uint32_t lineInd): lineInd(lineInd), indent(0) {}

const char OPERATOR_CHARSET[] = "~!@#$%^&*(){}[]?+\\|`-:<=>";

inline bool is_identifier_start(char c) {
	return isalpha(c);
}
inline bool is_identifier_body(char c) {
	return isalnum(c) || (c == '_');
}
bool is_operator(char c) {
	for (const char *p = OPERATOR_CHARSET;*p != '\0';p++) {
		if (*p == c) {
			return true;
		}
	}
	return false;
}

std::vector<Token>& concat_identifiers_funcdef(std::vector<Token>& tok) {
	uint32_t j = 1;
	for (uint32_t i = 1;i < tok.size();i++) {
		tok[j].charInd = tok[i].charInd;

		uint32_t arrCnt = 0;
		while ((i < tok.size()) && (tok[i].s == "#")) {
			arrCnt++;
			i++;
		}

		if (arrCnt == 0) {
			tok[j++] = tok[i];
			continue;
		} 
		tok[j].s = std::string(arrCnt, '#');
		tok[j].type = TokenType::Identifier;
		if ((i < tok.size()) && (tok[i].type == TokenType::Identifier)) {
			tok[j].s += tok[i].s;
		} else {
			i--;
		}
		j++;
	}
	tok.resize(j);
	return tok;
}
std::vector<Token>& concat_identifiers_vardef(std::vector<Token>& tok) {
	uint32_t i = 1;
	uint32_t arrCnt = 0;
	while ((i < tok.size()) && (tok[i].s == "#")) {
		arrCnt++;
		i++;
	}

	if (arrCnt > 0) {
		tok[1].s = std::string(arrCnt, '#');
		tok[1].type = TokenType::Identifier;
		if ((i < tok.size()) && (tok[i].type == TokenType::Identifier)) {
			tok[1].s += tok[i].s;
		} else {
			i--;
		}
	}
	uint32_t j = 2;
	for (i++;i < tok.size();i++) {
		tok[j++] = tok[i];
	}
	tok.resize(j);
	return tok;
}
std::vector<Token>& concat_identifiers_expr(std::vector<Token>& tok) {
	uint32_t j = 0;
	for (uint32_t i = 0;i < tok.size();i++) {
		tok[j++] = tok[i];

		if (tok[i].s != "%") {
			continue;
		} 
		i++;
		if (i < tok.size()) {
			tok[j].charInd = tok[i].charInd;
		}

		uint32_t arrCnt = 0;
		while ((i < tok.size()) && (tok[i].s == "#")) {
			arrCnt++;
			i++;
		}
		if (arrCnt == 0) {
			tok[j++] = tok[i];
			continue;
		}
		tok[j].s = std::string(arrCnt, '#');
		tok[j].type = TokenType::Identifier;
		if ((i < tok.size()) && (tok[i].type == TokenType::Identifier)) {
			tok[j].s += tok[i].s;
		} else {
			i--;
		}
		j++;
	}
	tok.resize(j);
	return tok;
}
std::vector<Token>& concat_identifiers(std::vector<Token> &tok) {
	if (tok.empty()) {
		return tok;
	}
	const TokenType& t = tok[0].type;
	if (t == TokenType::DefFunc) {
		return concat_identifiers_funcdef(tok);
	} else if ((t == TokenType::DefWeak) || (t == TokenType::DefVar)) {
		concat_identifiers_vardef(tok);
		concat_identifiers_expr(tok);
		return tok;
	} else {
		return concat_identifiers_expr(tok);
	}
}

std::vector<Line> split_lines(std::istream &in) {
	std::vector<Line> ans;

	Line cur(1);
	bool nonSpace = false;
	bool comment = false;
	uint32_t lineCnt = 1;

	while (in.peek() != EOF) {
		char c = in.get();

		if (c == '\n') {
			if (nonSpace) {
				ans.push_back(cur);
			}
			cur = Line(++lineCnt);
			nonSpace = false;
			comment = false;
			continue;
		}
		if (c == ';') {
			comment = true;
		}
		if (comment) {
			continue;
		}

		if (c == '\\') {
			c = in.get();
			if (c == EOF) {
				break;
			} else if (c == '\n') {
				lineCnt++;
				continue;
			} else {
				cur.s.push_back('\\');
				nonSpace = true;
			}
		}
		cur.s.push_back(c);
		if (isspace(c)) {
			if (!nonSpace) {
				cur.indent++;
			} 
		} else {
			nonSpace = true;
		}
	}
	if (nonSpace) {
		ans.push_back(cur);
	}
	return ans;
}

std::vector<Token> tokenize(const std::string &s) {
	std::vector<Token> ans;
	for (uint32_t i = 0;i < s.size();i++) {
		if (isspace(s[i])) {
			continue;
		}
		Token cur;
		cur.charInd = i;
		if (isdigit(s[i])) {
			cur.type = TokenType::LitInt;
			cur.s += s[i];
			while ((i + 1 < s.size()) && (isdigit(s[i + 1]))) {
				cur.s += s[++i];
			}
			ans.push_back(cur);
		} else if (is_identifier_start(s[i])) {
			cur.type = TokenType::Identifier;
			cur.s += s[i];
			while ((i + 1 < s.size()) && (is_identifier_body(s[i + 1]))) {
				cur.s += s[++i];
			}
			ans.push_back(cur);
		} else if (is_operator(s[i])) {
			cur.type = TokenType::Operator;
			cur.s = s[i];

			if ((s[i] == '#') || (s[i] == ':')) {
				ans.push_back(cur);
				continue;
			} 
			while ((i + 1 < s.size()) && (is_operator(s[i + 1]))) {
				cur.s += s[++i];
			}
			if (cur.s == "=>") {
				cur.type = TokenType::Return;
			} else if (cur.s == "$") {
				cur.type = TokenType::DefVar;
			} else if (cur.s == "$@") {
				cur.type = TokenType::DefFunc;
			}
			ans.push_back(cur);
		}
	}
	return concat_identifiers(ans);
}
