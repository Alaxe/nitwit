#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cctype>
#include <cassert>

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

	Token(uint32_t charInd = 0): type(TokenType::Unknown), charInd(charInd) {}
};

class Line {
public:
	std::string s;

	uint32_t lineInd;
	uint32_t indent;
	std::vector<Token> tokens;

	Line(uint32_t lineInd = 0): lineInd(lineInd), indent(0) {}
};

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

inline bool is_identifier_start(char c) {
	return isalpha(c);
}
inline bool is_identifier_body(char c) {
	return isalnum(c) || (c == '_');
}

const char OPERATOR_CHARSET[] = "~!@#$%^&*(){}[]?+\\|`-:<=>";

bool is_operator(char c) {
	for (const char *p = OPERATOR_CHARSET;*p != '\0';p++) {
		if (*p == c) {
			return true;
		}
	}
	return false;
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
	return ans;
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

class OperatorData {
public:
	std::string name;
	uint32_t arity;
	bool takeFloat;
	std::string cName; 

private:
	OperatorData(const std::string &name, uint32_t arity, bool takeFloat,
		const std::string &cName):
		name(name), arity(arity), takeFloat(takeFloat), cName(cName) {}
	
	static std::unordered_map<std::string, OperatorData> operators;

	static void init_if_needed() {
		if (!operators.empty()) {
			return;
		}
		operators.emplace("+", OperatorData("+", 2, true, "+"));
		operators.emplace("-", OperatorData("-", 2, true, "-"));
		operators.emplace("*", OperatorData("*", 2, true, "*"));
		operators.emplace("/", OperatorData("/", 2, true, "/"));
		operators.emplace("\\\\", OperatorData("\\\\", 2, false, "%"));
		
		operators.emplace("==", OperatorData("==", 2, true, "=="));
		operators.emplace("!=", OperatorData("!=", 2, true, "!="));
		operators.emplace(">=", OperatorData(">=", 2, true, ">="));
		operators.emplace("<=", OperatorData("<=", 2, true, "<="));
		operators.emplace(">", OperatorData(">", 2, true, ">"));
		operators.emplace("<", OperatorData("<", 2, true, "<"));

		operators.emplace("&", OperatorData("&", 2, false, "&&"));
		operators.emplace("|", OperatorData("|", 2, false, "||"));
		operators.emplace("~", OperatorData("~", 1, false, "!"));
	}
public:
	static const OperatorData* get(const std::string &name) {
		init_if_needed();

		auto it = operators.find(name);
		if (it != operators.end()) {
			return &it->second; //fine as once the map is constructed, it isn't modified
		} else {
			return nullptr;
		}
	}
};

std::unordered_map<std::string, OperatorData> OperatorData::operators;

class FuncProto {
public:
	uint32_t argCnt;
	FuncProto(uint32_t argCnt = 0): argCnt(argCnt) {}
};

class TypeT {};

class ResultT {
public:
	enum class Category {
		Value,
		Reference,
		Identifier,
		Unknown
	};
	TypeT type;
	Category category;
	ResultT(Category cat = Category::Unknown): category(cat) {}

	bool is_value() const {
		return (category == Category::Value)
			|| (category == Category::Reference);
	}
};

class Context {
private:
	std::unordered_map<std::string, FuncProto> functions;
	std::unordered_map<std::string, TypeT> globalVar;
	std::unordered_map<std::string, TypeT> localVar;
	
public:
	void declare_function(const std::string &name, const FuncProto &proto) {
		assert(functions.find(name) == functions.end());
		functions[name] = proto;
	}

	void declare_global_var(const std::string &name, const TypeT &type) {
		assert(globalVar.find(name) == globalVar.end());
		globalVar[name] = type;
	}

	void declare_local_var(const std::string &name, const TypeT &type) {
		assert(localVar.find(name) == localVar.end());
		localVar[name] = type;
	}
	void remove_locals() {
		localVar.clear();
	}

	const FuncProto* get_function(const std::string &name) const {
		auto it = functions.find(name);

		if (it != functions.end()) {
			return &it->second;
		} else {
			return nullptr;
		}
	}
	const TypeT* get_variable(const std::string &name) const {
		auto itLocal = localVar.find(name);

		if (itLocal != localVar.end()) {
			return &itLocal->second;
		} else {
			auto itGlobal = globalVar.find(name);
			if (itGlobal != globalVar.end()) {
				return &itGlobal->second;
			} else {
				return nullptr;
			}
		}
	}
};

Context generate_global_context(const std::vector<Line> &lines) {
	Context context;
	for (const Line &l : lines) {
		if (l.indent > 0) {
			continue;
		}
		const TokenType &tt = l.tokens[0].type;
		if (tt == TokenType::DefFunc) {
			if ((l.tokens.size() < 3) || (l.tokens.size() % 2 == 0)) {
				std::cerr << "Invalid number of tokens for func def on line " << l.lineInd << "\n";
				assert(false);
			}
			context.declare_function(l.tokens[2].s, FuncProto((l.tokens.size() - 3) / 2));

			std::cerr << "gfunc " << l.tokens[2].s << " " << (l.tokens.size() - 3) / 2 << "\n";

		} else if ((tt == TokenType::DefVar) || (tt == TokenType::DefWeak)) {
			if (l.tokens.size() < 3) {
				std::cerr << "Invalid number of args for var def on line " << l.lineInd << "\n"; 
				assert(false);
			} else if (l.tokens.size() > 3) {
				std::cerr << "Trailing tokens after var declaration on line " << l.lineInd << "\n";
				assert(false);
			}
			assert(l.tokens[2].type == TokenType::Identifier);

			context.declare_global_var(l.tokens[2].s, TypeT());

			std::cerr << "gvar " << l.tokens[2].s << "\n";
		}
	}
	return context;
}

class StatementAST {
public:
	virtual ~StatementAST() {}
	//a codegen virtual function
};

class ExprAST : StatementAST {
protected:
	ResultT resultType;
	typedef std::vector<std::unique_ptr<ExprAST>> Stack;

public:
	virtual ~ExprAST() {}
	const ResultT& get_result_type() {
		return resultType;
	}
	//an expression string generation function
};

class UnOperatorAST : public ExprAST {
protected:
	std::string opr;
	std::unique_ptr<ExprAST> operand;
};

class LitAST : public ExprAST {
private:
	std::string val;
public:
	LitAST(const Token &t): val(t.s) {
		resultType.category = ResultT::Category::Value;
	}
	const std::string& get_val() const {
		return val;
	}
};

class IdentifierAST : public ExprAST {
protected:
	std::string name;

public:
	IdentifierAST(const Token &t, const Context &context): name(t.s) {
		const TypeT *typePtr = context.get_variable(t.s);
		if (typePtr == nullptr) {
			resultType.category = ResultT::Category::Identifier;
		} else {
			resultType.category = ResultT::Category::Reference;
		}
	}
	const std::string& get_name() const {
		return name;
	}
};

class BinOperatorAST : public ExprAST {
protected:
	const OperatorData *oprData;
	std::unique_ptr<ExprAST> lhs, rhs;

public:
	BinOperatorAST(const Token &t, const Context &context, ExprAST::Stack &stack) {
		oprData = OperatorData::get(t.s);
		assert((oprData != nullptr) && (oprData->arity == 2));
		
		assert(stack.size() >= 2);
		lhs = std::move(stack.back());
		stack.pop_back();
		rhs = std::move(stack.back());
		stack.pop_back();
		
		//It's a normal operator, so it only requires its arguments to
		//values or references
		
		resultType.category = ResultT::Category::Value;

		assert(lhs->get_result_type().is_value());
		assert(rhs->get_result_type().is_value());
	}
};

class FunctionCallAST : public ExprAST {
protected:
	std::string funcName; //the nitwit one
	std::vector<std::unique_ptr<ExprAST>> args;

public:
	FunctionCallAST(const Token &t, const Context &context, ExprAST::Stack &stack) {
		assert(!stack.empty());

		IdentifierAST* funcIdent = dynamic_cast<IdentifierAST*> (stack.back().get());
		stack.pop_back();
		assert(funcIdent != nullptr);
		funcName = funcIdent->get_name();

		const FuncProto *proto = context.get_function(funcName);
		assert(proto != nullptr);
		assert(proto->argCnt <= stack.size());

		resultType.category = ResultT::Category::Value;

		for (uint32_t i = 0;i < proto->argCnt;i++) {
			const ResultT &argT = stack.back()->get_result_type();
			assert(argT.is_value()); //add a more thorough type check when, well, we have types
			args.push_back(std::move(stack.back())); 
			stack.pop_back();
		}
	}
};

class AssignmentAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> lhs, rhs;
public:
	AssignmentAST(const Token &t, const Context &context, ExprAST::Stack &stack) {
		assert (stack.size() >= 2);
		lhs = std::move(stack.back());
		stack.pop_back();
		rhs = std::move(stack.back());
		stack.pop_back();

		assert(lhs->get_result_type().category == ResultT::Category::Reference);
		assert(rhs->get_result_type().is_value());

		resultType.category = ResultT::Category::Value;
	}
};

class InputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	InputAST(const Token &t, const Context &context, ExprAST::Stack &stack) {
		assert (stack.size() > 1);
		operand = std::move(stack.back());
		stack.pop_back();

		assert(operand->get_result_type().category == ResultT::Category::Reference);
		resultType.category = ResultT::Category::Value;
	}
};

class OutputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	OutputAST(const Token &t, const Context &context, ExprAST::Stack &stack) {
		assert (stack.size() > 1);
		operand = std::move(stack.back());
		stack.pop_back();

		assert(operand->get_result_type().is_value());
		resultType.category = ResultT::Category::Value;
	}

};

class VarDefAST : public StatementAST {
private:
	std::string name;
	TypeT type;
public:
	VarDefAST(const Token &typeTok, const Token &nameTok) {
		assert(typeTok.type == TokenType::Identifier);
		assert(typeTok.s == "int");
		assert(nameTok.type == TokenType::Identifier);

		name = nameTok.s;
	}
};

class ReturnAST : public StatementAST {
private:
	std::unique_ptr<ExprAST> returnVal;
public:
	ReturnAST(std::unique_ptr<ExprAST> val): returnVal(std::move(val)) {
		assert(returnVal->get_result_type().is_value());
	}
};

class Function {
private:
	std::vector<std::string> args;
	std::vector<std::unique_ptr<StatementAST>> statements;
};

int main() {
	std::ifstream fin("sample.ntwt");

	auto code = split_lines(fin);
	for (auto &i : code) {
		i.tokens = tokenize(i.s);
		concat_identifiers(i.tokens);
		std::cout << i.lineInd << " (" << i.indent << "): ";
		//std::cout << i.s << "\n";
		for (auto &t : i.tokens) {
			std::cout << "[" << (uint32_t) (t.type) << " " << t.s << "] ";
		}
		std::cout << "\n";
	}
	//std::cout << OperatorData::get("~").cName << "\n";


	Context context = generate_global_context(code);
}
