#pragma once
#include "Pass.h"

#include <string_view>
#include <string>

namespace aec
{
    struct HelloPass : public BasePass<HelloPass> {};

    struct Evaluator : public AstReadPass<Evaluator>
    {
        void run(const AbstractSyntaxTree &);
        void operator()(const Expr &) override;
        void operator()(const Term &) override;
        void operator()(const Factor &) override;
        void operator()(const UnaryExpr<Term> &) override;
        void operator()(const UnaryExpr<Factor> &) override;
        void operator()(const Number &) override;

        int64_t result() const;

    private:
        int64_t eval(const Expr &);
        int64_t eval(const Term &);
        int64_t eval(const Factor &);
        int64_t eval(const Number &);

    private:
        int64_t res = 0;
    };

    /// dump the ast to string
    struct Printer : public AstReadPass<Printer>
    {
        void run(const AbstractSyntaxTree &);
        void operator()(const Expr &);
        void operator()(const Term &);
        void operator()(const Factor &);
        void operator()(const UnaryExpr<Term> &);
        void operator()(const UnaryExpr<Factor> &);
        void operator()(const Number &);

        std::string result() const;

    private:
        void dump_operator(const Operator &op);
        void record(const std::string_view &info);

    private:
        std::string buf = "";
    };

    // TODO:
    // /// transform any '*' expression node into '+' node
    // /// 积化和差
    // struct Prosthaphaeresis : public AstWritePass<Prosthaphaeresis>
    // {
    //     void pass(AbstractSyntaxTree&) override;
    //     void operator()(Expr &) override;
    //     void operator()(Term &) override;
    //     void operator()(Factor &) override;
    //     void operator()(UnaryExpr<Term> &) override;
    //     void operator()(UnaryExpr<Factor> &) override;
    //     void operator()(Number &) override;
    // };

}