#pragma once

#include <functional>

#include "Ast.h"

namespace aec
{
    using std::function;

    /// pass interface for schedule 
    struct PassTrait
    {
        virtual void execute(AbstractSyntaxTree &tree) = 0;
        virtual void result() = 0;
    };

    /// CRTP pass base class
    template <typename T>
    struct BasePass : public PassTrait
    {
    public:
        using HookFn = function<void(T &)>;
        optional<HookFn> before = std::nullopt;
        optional<HookFn> after = std::nullopt;

    public:
        void execute(AbstractSyntaxTree &tree) override
        {
            auto derived_ptr = static_cast<T *>(this);

            std::invoke(before.value_or([](auto& _) {}), *derived_ptr);
            derived_ptr->run(tree);
            std::invoke(after.value_or([](auto& _) {}), *derived_ptr);
        }

        void result()
        {
        }

        /// interface to pass through an AST, need to be implmented in derivied class 
        void run(AbstractSyntaxTree &tree)
        {
        }

/// equivalent to this statement,
/// but it can't compile successfully 
/// std::visit(*this, field)
#define self_visit(field) this->operator()(field)

    protected:
        /// these functions is for self_visit macro 
        void operator()(Expr &)
        {
        }

        void operator()(Term &)
        {
        }

        void operator()(Factor &)
        {
        }

        void operator()(UnaryExpr<Term> &)
        {
        }

        void operator()(UnaryExpr<Factor> &)
        {
        }

        void operator()(Number &)
        {
        }
    };

    /// operations that will *NOT* change the AST
    template <typename T>
    struct AstReadPass : public BasePass<T>
    {
    protected:
        virtual void operator()(const Expr&) = 0;
        virtual void operator()(const Term &) = 0;
        virtual void operator()(const Factor &) = 0;
        virtual void operator()(const UnaryExpr<Term> &) = 0;
        virtual void operator()(const UnaryExpr<Factor> &) = 0;
        virtual void operator()(const Number &) = 0;
    };

    /// operations that will change the AST
    template <typename T>
    struct AstWritePass : public BasePass<T>
    {
    };

    template <class... Types>
    struct OverloadedPass : Types...
    {
        using Types::operator()...;
    };

    template <class... Types>
    OverloadedPass(Types...) -> OverloadedPass<Types...>;

}
// end namespace aec
 