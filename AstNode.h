#pragma once

#include "Lexer.h"

#include <memory>

namespace aec
{
    using std::unique_ptr;

    /// forward declare all struct of ast node
    ///
    /// struct Expr;
    /// struct Term;
    /// struct Factor;
    ///
    /// template<>
    /// struct BinaryExpr<Term>;
    ///
    /// template<>
    /// struct UnaryExpr<Factor>;
    ///

    template <typename... Types>
    using Maybe = variant<Types...>;

    template <typename T>
    using UniqueOwn = unique_ptr<T>;

    template <typename T>
    using Repeated = vector<T>;

    template <typename T>
    struct UnaryExpr
    {
        Operator op;
        T operand;
    };

    struct Term;
    struct Expr
    {
        using Head = Term;
        using Body = UnaryExpr<Term>;

        UniqueOwn<Head> term;
        Repeated<UniqueOwn<Body>> follows;
    };

    struct Factor;
    struct Term
    {
        using Head = Factor;
        using Body = UnaryExpr<Factor>;

        UniqueOwn<Head> factor;
        Repeated<UniqueOwn<Body>> follows;
    };

    struct Factor
    {
        using Value = Maybe<Expr, Number>;
        UniqueOwn<Value> expr_or_num;
    };

    using ValidAstNode = variant<Expr,
                                 Term,
                                 Factor>;

    template <typename T>
    concept OneOfAstNodeType = requires(T _)
    {
        std::is_same<T, Expr>() || std::is_same<T, Term>() || std::is_same<T, Factor>() 
        || std::is_same<T, UnaryExpr<Factor>>() 
        || std::is_same<T, UnaryExpr<Term>>();
    };

    class AstNode
    {
    public:
        AstNode() = default;
        AstNode(const AstNode &) = default;
        AstNode(AstNode &&) = default;
        ~AstNode() = default;

    public:
        AstNode(ValidAstNode &&);

    public:
        template <typename T>
        bool is() const noexcept
        {
            return std::holds_alternative<T>(_node);
        }

        ValidAstNode &raw()
        {
            return _node;
        }

    private:
        ValidAstNode _node;
    };

} // end namespace aec