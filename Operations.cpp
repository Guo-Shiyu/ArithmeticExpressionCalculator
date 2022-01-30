#include "Operations.h"

#include <iostream>

using namespace aec;

void aec::Evaluator::run(const AbstractSyntaxTree &tree)
{
    std::visit(*this, tree.root().raw());
}

void Evaluator::operator()(const Expr &expr)
{
    res += eval(expr);
}

void Evaluator::operator()(const Term &term)
{
    res += eval(term);
}

void Evaluator::operator()(const Factor &factor)
{
    res += eval(factor);
}

void Evaluator::operator()(const UnaryExpr<Term> &unary_expr)
{
    /// unreachable code
}

void Evaluator::operator()(const UnaryExpr<Factor> &unary_expr)
{
    /// unreachable code
}

void Evaluator::operator()(const Number &num)
{
    res += eval(num);
}

int64_t aec::Evaluator::eval(const Expr &expr)
{
    auto val = eval(*expr.term);
    for (auto &term : expr.follows)
        switch (term->op)
        {
        case Operator::Add:
            val += eval(term->operand);
            break;

        case Operator::Sub:
            val -= eval(term->operand);
            break;

        default: /// unreachable code
            break;
        }

    return val;
}

int64_t aec::Evaluator::eval(const Term &term)
{
    auto val = eval(*term.factor);
    for (auto &factor : term.follows)
        switch (factor->op)
        {
        case Operator::Mul:
            val *= eval(factor->operand);
            break;
        case Operator::Div:
            val /= eval(factor->operand);
            break;

        default: /// unreachable code
            break;
        }

    return val;
}

int64_t aec::Evaluator::eval(const Factor &factor)
{
    int64_t val{};
    std::visit(OverloadedPass{[this, &val](const Expr &expr)
                              {
                                  val = eval(expr);
                              },

                              [this, &val](const Number &num)
                              {
                                  val = num;
                              }},
               *factor.expr_or_num);
    return val;
}

int64_t aec::Evaluator::eval(const Number &num)
{
    return num;
}

int64_t Evaluator::result() const
{
    return res;
}

void aec::Printer::run(const AbstractSyntaxTree &tree)
{
    std::visit(*this, tree.root().raw());
    if (buf.ends_with(' '))
        buf.pop_back();
}

void Printer::operator()(const Expr &expr)
{
    self_visit(*expr.term);
    for (auto &term : expr.follows)
        self_visit(*term);
}

void Printer::operator()(const Term &term)
{
    self_visit(*term.factor);
    for (auto &factor : term.follows)
        self_visit(*factor);
}

void Printer::operator()(const Factor &factor)
{
    std::visit(OverloadedPass{[this](const Expr &expr)
                              {
                                  record("(");
                                  self_visit(expr);
                                  record(")");
                              },

                              [this](const Number &num)
                              {
                                  self_visit(num);
                              }},
               *factor.expr_or_num);
}

void Printer::operator()(const UnaryExpr<Term> &unary_expr)
{
    dump_operator(unary_expr.op);
    self_visit(unary_expr.operand);
}

void Printer::operator()(const UnaryExpr<Factor> &unary_expr)
{
    dump_operator(unary_expr.op);
    self_visit(unary_expr.operand);
}

void Printer::operator()(const Number &num)
{
    record(std::to_string(num));
}

void Printer::dump_operator(const aec::Operator &op)
{
    using aec::Operator;
    std::string_view opstr;
    switch (op)
    {
    case Operator::Add:
        opstr = "+";
        break;
    case Operator::Sub:
        opstr = "-";
        break;
    case Operator::Mul:
        opstr = "*";
        break;
    case Operator::Div:
        opstr = "/";
        break;

    default: /// unreachable code

        break;
    }
    record(opstr);
}

void Printer::record(const string_view &info)
{
    this->buf.append(info).push_back(' ');
}

string Printer::result() const
{
    return buf;
}

// void aec::Prosthaphaeresis::run(AbstractSyntaxTree&)
// {

// }

// void Prosthaphaeresis::operator()(Expr&)
// {
// }

// void Prosthaphaeresis::operator()(Term&)
// {
// }

// void Prosthaphaeresis::operator()(Factor&)
// {
// }

// void Prosthaphaeresis::operator()(UnaryExpr<Term>&)
// {
// }

// void Prosthaphaeresis::operator()(Number&)
// {
// }

// void Prosthaphaeresis::operator()(UnaryExpr<Factor>&)
// {
// }
