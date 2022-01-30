#include "Ast.h"

using namespace aec;

#define AST AbstractSyntaxTree

AST::AbstractSyntaxTree(AstNode &&node)
	: _root(std::make_unique<AstNode>(std::move(node)))
{
}

AST::AstRoot &AST::root() const
{
	return *_root;
}

void AST::check_eof(Lexer &lex)
{
	if (lex.is_eof())
		unexpect<void>({"unexpected eof"});
}

AST AST::parse(string &&expr)
{
	auto lexer = Lexer{expr};
	auto ret = AST(AstNode{parse_expr(lexer)});
	if (not lexer.is_eof())
		unexpect<void>("unfinished parsing");
	return ret;
}

/// expr ::= term { ("+"|"-") term }*
Expr AST::parse_expr(Lexer &lex)
{
	Expr expr;

	/// parse first term
	expr.term = std::make_unique<Expr::Head>(parse_term(lex));

	/// parse repeated term
	while (not lex.is_eof())
		if (auto next = lex.peek();
			is_expected(next, Operator::Add) || is_expected(next, Operator::Sub))
			expr.follows.push_back(std::make_unique<Expr::Body>(UnaryExpr<Term>{
				.op = lex.next().get_unchecked<Operator>(),
				.operand = parse_term(lex),
			}));
		else
			break;

	return expr;
}

/// term ::= factor { ( "*" | "/" ) factor }*
Term AST::parse_term(Lexer &lex)
{
	Term term;

	/// parse first factor
	term.factor = std::make_unique<Term::Head>(parse_factor(lex));

	/// parse repeated factor
	while (not lex.is_eof())
		if (auto next = lex.peek();
			is_expected(next, Operator::Mul) || is_expected(next, Operator::Div))
			term.follows.push_back(std::make_unique<Term::Body>(UnaryExpr<Factor>{
				.op = lex.next().get_unchecked<Operator>(),
				.operand = parse_factor(lex),
			}));
		else
			break;

	return term;
}

/// factor ::= "(" expr ")" | NUM
Factor AST::parse_factor(Lexer &lex)
{
	check_eof(lex);
	auto next = lex.next();
	if (is_expected(next, Delim::LeftParent))
	{
		auto expr = parse_expr(lex);
		if (auto next = lex.next(); is_expected(next, Delim::RightParent)) /// skip right parent
			return {std::make_unique<Factor::Value>(std::move(expr))};
		else
			return unexpect<Factor>("'right parent' expected here");
	}
	else if (next.is<Number>())
		return {std::make_unique<Factor::Value>(next.get_unchecked<Number>())};
	else
		return unexpect<Factor>("'number' or 'expr' expected here, but xxxx was found");
}

#undef AST