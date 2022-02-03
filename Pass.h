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
        using AsSuper = BasePass<T>;
        using HookFn = function<void(T &)>;

        optional<HookFn> before = std::nullopt;
        optional<HookFn> after = std::nullopt;

        void execute(AbstractSyntaxTree &tree) override
        {
            auto derived_ptr = static_cast<T *>(this);

            std::invoke(before.value_or([](auto &_) {}), *derived_ptr);
            derived_ptr->run(tree);
            std::invoke(after.value_or([](auto &_) {}), *derived_ptr);
        }

        void result()
        {
        }

        /// interface to pass through an AST, need to be implmented in derivied class
        void run(AbstractSyntaxTree &tree)
        {
            std::visit(*static_cast<T*>(this), tree.root().raw());
        }

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

/// equivalent to this statement,
/// but it can't compile successfully
/// std::visit(*this, field)
#define self_visit(field) this->operator()(field)

    /// pass that will change the AST
    template <typename T>
    using AstWritePass = BasePass<T>;

    /// that will *NOT* change the AST
    template <typename T>
    struct AstReadPass : public BasePass<T>
    {
        using AsSuper = AstReadPass<T>;
        void run(const AbstractSyntaxTree& tree)
        {
            std::visit(*static_cast<T*>(this), tree.root().raw());
        }

        void operator()(const Expr&)
        {
        }

        void operator()(const Term&)
        {
        }

        void operator()(const Factor&)
        {
        }

        void operator()(const UnaryExpr<Term>&)
        {
        }

        void operator()(const UnaryExpr<Factor>&)
        {
        }

        void operator()(const Number&)
        {
        }
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
