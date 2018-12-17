#include <algorithm>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>
#include <unordered_map>
#include <cctype>
#include <cassert>

#include "lexer.h"

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
		RValue,
		LValue,
		Identifier,
		Unknown
	};
	TypeT type;
	Category category;
	ResultT(Category cat = Category::Unknown): category(cat) {}

	bool is_value() const {
		return (category == Category::RValue)
			|| (category == Category::LValue);
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
	virtual void debug_print() const {}
};

class ExprAST : public StatementAST {
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
		resultType.category = ResultT::Category::RValue;
	}
	const std::string& get_val() const {
		return val;
	}

	virtual void debug_print() const {
		std::cerr << "Literal " << val << "\n";
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
			resultType.category = ResultT::Category::LValue;
		}
	}
	const std::string& get_name() const {
		return name;
	}

	virtual void debug_print() const {
		std::cerr << "Identifier ";
		if (resultType.category == ResultT::Category::LValue) {
			std::cerr << "(LValue) ";
		}
		std::cerr << name << "\n";
	}
};

class BinOperatorAST : public ExprAST {
protected:
	const OperatorData *oprData;
	std::unique_ptr<ExprAST> lhs, rhs;

public:
	BinOperatorAST(const OperatorData *oprData, ExprAST::Stack &stack):
		oprData(oprData) {

		assert((oprData != nullptr) && (oprData->arity == 2));

		assert(stack.size() >= 2);
		lhs = std::move(stack.back());
		stack.pop_back();
		rhs = std::move(stack.back());
		stack.pop_back();

		//It's a normal operator, so it only requires its arguments to
		//values or references

		resultType.category = ResultT::Category::RValue;

		assert(lhs->get_result_type().is_value());
		assert(rhs->get_result_type().is_value());
	}

	virtual void debug_print() const {
		std::cerr << "binary operator " << oprData->name << "\n";
		std::cerr << "first argument (\n";
		lhs->debug_print();
		std::cerr << ")\n";
		std::cerr << "second argument " << oprData->name << "(\n";
		rhs->debug_print();
		std::cerr << ")\n";
	}
};

class FunctionCallAST : public ExprAST {
protected:
	std::string funcName; //the nitwit one
	std::vector<std::unique_ptr<ExprAST>> args;

public:
	FunctionCallAST(const Context &context, ExprAST::Stack &stack) {
		assert(!stack.empty());

		std::unique_ptr<IdentifierAST> funcIdent(
			dynamic_cast<IdentifierAST*> (stack.back().release())
		);
		stack.pop_back();
		assert(funcIdent != nullptr);
		funcName = funcIdent->get_name();


		const FuncProto *proto = context.get_function(funcName);
		assert(proto != nullptr);
		assert(proto->argCnt <= stack.size());

		resultType.category = ResultT::Category::RValue;

		for (uint32_t i = 0;i < proto->argCnt;i++) {
			const ResultT &argT = stack.back()->get_result_type();
			assert(argT.is_value()); //add a more thorough type check when, well, we have types
			args.push_back(std::move(stack.back()));
			stack.pop_back();
		}
	}
	virtual void debug_print() const {
		std::cerr << "function call " << funcName << " with " << args.size() << "arguments.\n";
		for (uint32_t i = 0;i < args.size();i++) {
			std::cerr << "argument " << i + 1 << " (" << funcName << ") (\n";
			args[i]->debug_print();
			std::cerr << ")\n";
		}
	}
};

class AssignmentAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> lhs, rhs;
public:
	AssignmentAST(ExprAST::Stack &stack) {
		assert (stack.size() >= 2);
		lhs = std::move(stack.back());
		stack.pop_back();
		rhs = std::move(stack.back());
		stack.pop_back();

		assert(lhs->get_result_type().category == ResultT::Category::LValue);
		assert(rhs->get_result_type().is_value());

		resultType.category = ResultT::Category::RValue;
	}

	virtual void debug_print() const {
		std::cerr << "assignment \n";
		std::cerr << "first argument (\n";
		lhs->debug_print();
		std::cerr << ")\n";
		std::cerr << "second argument (assignment) (\n";
		rhs->debug_print();
		std::cerr << ")\n";
	}
};

class InputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	InputAST(ExprAST::Stack &stack) {
		assert (!stack.empty());
		operand = std::move(stack.back());
		stack.pop_back();

		assert(operand->get_result_type().category == ResultT::Category::LValue);
		resultType.category = ResultT::Category::RValue;
	}
	virtual void debug_print() const {
		std::cerr << "input >> (\n";
		operand->debug_print();
		std::cerr << ")\n";
	}
};

class OutputAST : public ExprAST {
protected:
	std::unique_ptr<ExprAST> operand;
public:
	OutputAST(ExprAST::Stack &stack) {
		assert (!stack.empty());
		operand = std::move(stack.back());
		stack.pop_back();

		assert(operand->get_result_type().is_value());
		resultType.category = ResultT::Category::RValue;
	}
	virtual void debug_print() const {
		std::cerr << "output >> (\n";
		operand->debug_print();
		std::cerr << ")\n";
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

	void add_to_context(Context &context) {
		context.declare_local_var(name, type);
	}
	virtual void debug_print() const {
		std::cerr << "vardef (no type data) " << name << "\n";
	}

};

class ReturnAST : public StatementAST {
private:
	std::unique_ptr<ExprAST> returnVal;
public:
	ReturnAST(std::unique_ptr<ExprAST> val): returnVal(std::move(val)) {
		assert(returnVal->get_result_type().is_value());
	}

	virtual void debug_print() const {
		std::cerr << "Return (\n";
		returnVal->debug_print();
		std::cerr << ")\n";
	}
};

std::vector<std::unique_ptr<ExprAST>> parse_expression(
	std::vector<Token>::const_iterator begin,
	std::vector<Token>::const_iterator end,
	const Context &context) {

	std::vector<std::unique_ptr<ExprAST>> stack;
	for (auto it = end - 1;it + 1 != begin;it--) {
		const Token &tok = *it;
		if (tok.type == TokenType::LitInt) {
			stack.emplace_back(new LitAST(tok));
		} else if (tok.type == TokenType::Identifier) {
			stack.emplace_back(new IdentifierAST(tok, context));
		} else {
			assert(tok.type == TokenType::Operator);
			const OperatorData* oprData = OperatorData::get(tok.s);

			if (oprData != nullptr) {
				if (oprData->arity == 2) {
					stack.emplace_back(new BinOperatorAST(oprData, stack));
				} else {
					assert(oprData->arity == 2);
				}
			} else if (tok.s == "@") {
				stack.emplace_back(new FunctionCallAST(context, stack));
			} else if (tok.s == "=") {
				stack.emplace_back(new AssignmentAST(stack));
			} else if (tok.s == ">>") {
				stack.emplace_back(new InputAST(stack));
			} else if (tok.s == "<<") {
				stack.emplace_back(new OutputAST(stack));
			}
		}
	}

	std::reverse(stack.begin(), stack.end());
	return stack;
}

class Function {
private:
	std::vector<std::string> args;
	std::vector<std::unique_ptr<StatementAST>> statements;
public:
	Function(std::vector<Line>::const_iterator begin,
		std::vector<Line>::const_iterator end,
		Context context) {

		assert(begin->tokens[0].type == TokenType::DefFunc);
		assert(begin->tokens.size() % 2 == 1);

		for (uint32_t i = 1;i < begin->tokens.size();i++) {
			assert(begin->tokens[i].type == TokenType::Identifier);
			if (i % 2) {
				assert(begin->tokens[i].s == "int");
			} else {
				args.push_back(begin->tokens[i].s);
				context.declare_local_var(args.back(), TypeT());
			}
		}

		bool bodyStarted = false;
		for (auto lineIt = begin + 1;lineIt != end;lineIt++) {
			auto tok = lineIt[0].tokens;
			if (tok[0].type == TokenType::DefVar) {
				assert(tok.size() == 3);
				assert(!bodyStarted);

				std::unique_ptr<VarDefAST> ptr(new VarDefAST(tok[1], tok[2]));
				ptr->add_to_context(context);
				statements.push_back(std::move(ptr));
			} else if (tok[0].type == TokenType::Return) {
				auto expr = parse_expression(tok.begin() + 1, tok.end(), context);
				assert(expr.size() == 1);
				std::unique_ptr<ReturnAST> ptr(new ReturnAST(std::move(expr[0])));
				statements.push_back(std::move(ptr));
				bodyStarted = true;
			} else {
				auto expr = parse_expression(tok.begin(), tok.end(), context);
				for (auto &i : expr) {
					std::unique_ptr<StatementAST> ptr(
						static_cast<StatementAST*>(i.release())
					);
					statements.push_back(std::move(ptr));
				}

				bodyStarted = true;
			}
			statements.back()->debug_print();
			std::cerr << "-----------------" << std::endl;
		}
	}
};

std::vector<Function> build_functions(const std::vector<Line> &code, const Context &context) {
	std::vector<Function> functions;
	for (uint32_t i = 0;i < code.size();i++) {
		assert(code[i].indent == 0);
		if (code[i].tokens[0].type == TokenType::DefVar) {
			continue;
		}
		assert(code[i].tokens[0].type == TokenType::DefFunc);
		uint32_t j;
		for (j = i + 1;j < code.size();j++) {
			if (code[j].indent == 0) {
				break;
			}
		}
		functions.push_back(Function(code.begin() + i, code.begin() + j, context));
		i = j - 1;
	}
	return functions;
}

int main() {
	std::ifstream fin("sample.ntwt");

	auto code = split_lines(fin);
	for (auto &i : code) {
		i.tokens = tokenize(i.s);
		std::cout << i.lineInd << " (" << i.indent << "): ";
		//std::cout << i.s << "\n";
		for (auto &t : i.tokens) {
			std::cout << "[" << (uint32_t) (t.type) << " " << t.s << "] ";
		}
		std::cout << "\n";
	}
	//std::cout << OperatorData::get("~").cName << "\n";

	Context context = generate_global_context(code);
	std::vector<Function> functions = build_functions(code, context);
}
