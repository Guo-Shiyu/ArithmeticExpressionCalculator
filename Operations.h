#pragma once
#include "Pass.h"

namespace aec
{
    struct HelloPass : public BasePass<HelloPass> 
    {
        /// will print 'hello pass' when processing an expr
        void run(AbstractSyntaxTree& tree) const;
    };

    struct Evaluator : public AstReadPass<Evaluator>
    {
        void run(const AbstractSyntaxTree &);
        void operator()(const Expr &);
        void operator()(const Term &);
        void operator()(const Factor &);
        void operator()(const Number &);

        int64_t result() const;

    private:
        int64_t eval(const Expr &);
        int64_t eval(const Term &);
        int64_t eval(const Factor &);
        int64_t eval(const Number &);

    private:
        int64_t res = 0;
    };

    /// dump the ast to formatted string 
    struct Printer : public AstReadPass<Printer>
    {
        void run(const AbstractSyntaxTree &);
        void operator()(const Expr &);
        void operator()(const Term &);
        void operator()(const Factor &);
        void operator()(const UnaryExpr<Term> &);
        void operator()(const UnaryExpr<Factor> &);
        void operator()(const Number &);

        string result() const;

    private:
        void record(const Operator &op);
        void record(const std::string &info);

    private:
        string buf = "";
    };

     /// 积化和差
     /// transform any '*' expression node into '+' node
     struct Prosthaphaeresis : public AstWritePass<Prosthaphaeresis>
     {
         void operator()(Expr &);
         void operator()(Term &);
         void operator()(Factor &);
     };

}