#include "lexer.h"

#include <cassert>
#include <iomanip>

std::unordered_map<std::string, uint32_t> TokenType::tokStrings = {
	{"$", TokenType::VarDef},
	{"$@", TokenType::FuncDef},
	{"$:", TokenType::StructDef},
	{"=>", TokenType::Return},
	{"?", TokenType::If},
	{"!?", TokenType::Elif},
	{"!", TokenType::Else},
	{"[]", TokenType::While},
	{"!!", TokenType::Break},
	{"!~", TokenType::Continue},

	{"=", TokenType::Assign},
	{"==", TokenType::Equal},
	{"~=", TokenType::NotEqual},
	{":", TokenType::Member},
	{"#", TokenType::Index},
	{"%", TokenType::Alloc},
	{"|#|", TokenType::ArrSize},
	{"@", TokenType::FuncCall},
	{">>", TokenType::Read},
	{"<<", TokenType::Write},

	{"+", TokenType::Plus},
	{"-", TokenType::Minus},
	{"*", TokenType::Times},
	{"/", TokenType::Divide},
	{"//", TokenType::Remainder},
	{"<", TokenType::Less},
	{">", TokenType::Greater},
	{"<=", TokenType::LessEq},
	{">=", TokenType::GreatEq},
	{"&", TokenType::And},
	{"|", TokenType::Or},
	{"^", TokenType::Xor},

	{"~", TokenType::Not},
};

TokenType::TokenType(uint32_t val): val(val){}
TokenType TokenType::type_from_string(const std::string &s) {
	auto it = tokStrings.find(s);
	if (it == tokStrings.end()) {
		return TokenType::Unknown;
	} else {
		return it->second;
	}
}

TokenType::operator uint32_t () const {
	return val;
}

Token::Token(uint32_t startInd):
	type(TokenType::Unknown),
	startInd(startInd),
	endInd(0) {}

const char SPEC_SEQUENCE_CHARSET[] = "~!@#$%^&*(){}[]?+\\/|`-:<=>";
inline bool is_literal(char c) {
	return isdigit(c) || c == '.';
}
inline bool is_identifier_start(char c) {
	return isalpha(c);
}
inline bool is_identifier_body(char c) {
	return isalnum(c) || (c == '_');
}
bool is_spec_sequence(char c) {
	for (const char *p = SPEC_SEQUENCE_CHARSET;*p != '\0';p++) {
		if (*p == c) {
			return true;
		}
	}
	return false;
}

std::vector<Token> base_tokenization(const std::string &s) {
	std::vector<Token> ans;
	for (uint32_t i = 0;i < s.size();i++) {
		if (isspace(s[i])) {
			continue;
		}
		Token cur(i);
		if (is_literal(s[i])) {
			uint32_t dotCnt = s[i] == '.';
			bool invalid = false;

			cur.s += s[i];
			while ((i + 1 < s.size())
				&& !isspace(s[i + 1])
				&& !is_spec_sequence(s[i + 1])
			) {
				cur.s += s[++i];
				if (s[i] == '.') {
					dotCnt++;
				} else if (!isdigit(s[i])) {
					invalid = true;
				}
			}

			if (dotCnt == 0) {
				cur.type = TokenType::LitInt;
			} else if (dotCnt == 1) {
				if (cur.s.size() > 1) {
					cur.type = TokenType::LitFloat;
				} else {
					cur.type = TokenType::LitNull;
				}
			} else {
				cur.type = TokenType::Unknown;
			}
			if (invalid) {
				cur.type = TokenType::Unknown;
			}
		} else if (is_identifier_start(s[i])) {
			cur.type = TokenType::Identifier;
			cur.s += s[i];

			while ((i + 1 < s.size())
				&& !isspace(s[i + 1])
				&& !is_spec_sequence(s[i + 1])
			) {
				cur.s += s[++i];
				if (!is_identifier_body(s[i])) {
					cur.type = TokenType::Unknown;
				}
			}
		} else if (is_spec_sequence(s[i])) {
			cur.s = s[i];

			while (
				(i + 1 < s.size())
				&& (is_spec_sequence(s[i + 1]))
			) {
				cur.s += s[++i];
			}

			cur.type = TokenType::type_from_string(cur.s);
			if (cur.type == TokenType::Unknown) {
				cur.type = TokenType::KeySeq;
			}
		}

		cur.endInd = i + 1;
		ans.push_back(cur);
	}
	return ans;
}

bool can_concat_tokens(const Token &a, const Token &b) {
	bool ans = (a.type & TokenType::KeySeq)
		&& (b.type == TokenType::Identifier);
	return ans;
}

Token concat_tokens(Token a, const Token &b) {
	a.s += b.s;
	a.endInd = b.endInd;
	a.type = TokenType::Identifier;
	return a;
}

std::vector<Token> concat_identifiers_funcdef(std::vector<Token> tok) {
	std::vector<Token> ans;
	ans.push_back(tok[0]);
	for (uint32_t i = 1;i < tok.size();i++) {
		if (ans.size() % 2 == 0) {
			ans.push_back(tok[i]);
			continue;
		}
		if (
			(i + 1 < tok.size())
			&& (can_concat_tokens(tok[i], tok[i + 1]))
		) {
			ans.push_back(concat_tokens(
				std::move(tok[i]),
				tok[i + 1]
			));
			i++;
		} else {
			ans.push_back(std::move(tok[i]));
		}
	}
	return ans;
}
std::vector<Token> concat_identifiers_vardef(std::vector<Token> tok) {
	if ((tok.size() >= 3) && (can_concat_tokens(tok[1], tok[2]))) {
		tok[1] = concat_tokens(std::move(tok[1]), tok[2]);
		tok.erase(tok.begin() + 2);
	}
	return tok;
}
std::vector<Token> concat_identifiers_expr(std::vector<Token> tok) {
	std::vector<Token> ans;
	for (uint32_t i = 0;i < tok.size();i++) {
		ans.push_back(std::move(tok[i]));

		if (ans.back().type != TokenType::Alloc) {
			continue;
		}
		if (
			(i + 2 < tok.size())
			&& (can_concat_tokens(tok[i + 1], tok[i + 2]))
		) {
			ans.push_back(concat_tokens(
				std::move(tok[i + 1]),
				tok[i + 2])
			);
			i += 2;
		};
	}
	return ans;
}
std::vector<Token> concat_identifiers(std::vector<Token> tok) {
	if (tok.empty()) {
		return tok;
	}
	const TokenType& t = tok[0].type;
	if (t == TokenType::FuncDef) {
		tok = concat_identifiers_funcdef(std::move(tok));
	} else if (t == TokenType::VarDef) {
		tok = concat_identifiers_vardef(std::move(tok));
		tok = concat_identifiers_expr(std::move(tok));
	} else {
		tok = concat_identifiers_expr(std::move(tok));
	}


	return tok;
}

std::vector<Token> split_tokens(std::vector<Token> tok) {
	std::vector<Token> ans;
	for (Token &t : tok) {
		uint32_t i;
		if ((t.type & TokenType::KeySeq) == 0) {
			ans.push_back(t);
			continue;
		}
		for (i = 0;i < t.s.size();i++) {
			if ((t.s[i] == '#') || (t.s[i] == ':')) {
				Token n;

				n.startInd = t.startInd + i;
				n.endInd = n.startInd + 1;
				n.s = t.s[i];
				n.type = TokenType::type_from_string(n.s);
				ans.push_back(n);
			} else {
				break;
			}
		}
		if (i < t.s.size()) {
			t.s.erase(0, i);
			ans.push_back(std::move(t));
		}
	}
	return ans;
}

std::vector<Token> Token::tokenize(const std::string &s) {
	std::vector<Token> tok = base_tokenization(s);
	tok = concat_identifiers(std::move(tok));
	tok = split_tokens(std::move(tok));
	return tok;
}
std::ostream& operator<< (std::ostream &out, const Token &t) {
	out << "[0x" << std::hex << (uint32_t) (t.type) << std::dec;
	return out << " " << t.s << "]";
}

Line::Line(uint32_t lineInd): lineInd(lineInd), indent(0) {}
std::vector<Line> Line::split_stream(std::istream &in) {
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
				cur.s.push_back(' ');
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
				if (c == '\t') {
					cur.indent += 4;
				} else {
					cur.indent++;
				}
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

std::ostream& operator<< (std::ostream &out, const Line &l) {
	out<< l.lineInd << " (" << l.indent << "): ";
	for (auto &t : l.tokens) {
		out << t << " ";
	}
	return out;
}
