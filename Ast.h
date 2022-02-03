#pragma once

#include "AstNode.h"

namespace aec
{
	using std::optional;

	template <typename T>
	concept AstVisitor = requires(T visitor, ValidAstNode node)
	{
		/// as same as these statements
		/// {visitor.operator()(node)};
		/// {visitor(node)};
		{std::visit(visitor, node)};
	};

	class AbstractSyntaxTree
	{
	public:
		using AstRoot = AstNode;

	private:
		AbstractSyntaxTree() = delete;

	public:
		AbstractSyntaxTree(const AbstractSyntaxTree &) = delete;
		AbstractSyntaxTree(AbstractSyntaxTree &&) = default;
		~AbstractSyntaxTree() = default;

	public:
		static AbstractSyntaxTree parse(string &&expr);
		static Expr parse_expr(Lexer &lex);
		static Term parse_term(Lexer &lex);
		static Factor parse_factor(Lexer &lex);

		AstRoot &root() const;

	protected:
		explicit AbstractSyntaxTree(AstNode &&);

		static void _check_eof(Lexer &lex);
	private:
		UniqueOwn<AstRoot> _root;
	};
}

#define is_expected(token, target) \
	token.is<decltype(target)>() && token.get_unchecked<decltype(target)>() == target