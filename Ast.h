#pragma once

#include "AstNode.h"

#include <optional>
#include <format>

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

	/// forward declare
	template <typename T>
	struct AstReadPass;
	template <typename T>
	struct AstWritePass;

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

		AstRoot &root() const;

	protected:
		explicit AbstractSyntaxTree(AstNode &&);

		static void check_eof(Lexer &lex);
		static Expr parse_expr(Lexer &lex);
		static Term parse_term(Lexer &lex);
		static Factor parse_factor(Lexer &lex);

	private:
		/// on error case
		template <typename T, typename... Args>
		static T unexpect(const char *fmt, Args... args)
		{
			throw std::format(fmt, args...);
			return T();
		}

	private:
		UniqueOwn<AstRoot> _root;
	};
}

#define is_expected(token, target) \
	token.is<decltype(target)>() && token.get_unchecked<decltype(target)>() == target