#include "Operations.h"

#include <iostream>

using namespace aec;

void HelloPass::run(AbstractSyntaxTree& tree) const
{
	std::cout << "hello pass" << std::endl;
}

void Evaluator::run(const AbstractSyntaxTree& tree)
{
	res = 0;
	AsSuper::run(tree);
}

void Evaluator::operator()(const Expr& expr)
{
	res += eval(expr);
}

void Evaluator::operator()(const Term& term)
{
	res += eval(term);
}

void Evaluator::operator()(const Factor& factor)
{
	res += eval(factor);
}

void Evaluator::operator()(const Number& num)
{
	res += eval(num);
}

int64_t aec::Evaluator::eval(const Expr& expr)
{
	auto val = eval(*expr.term);
	for (auto& term : expr.follows)
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

int64_t aec::Evaluator::eval(const Term& term)
{
	auto val = eval(*term.factor);
	for (auto& factor : term.follows)
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

int64_t aec::Evaluator::eval(const Factor& factor)
{
	int64_t val{};
	std::visit(OverloadedPass{ [this, &val](const Expr& expr)
							  {
								  val = eval(expr);
							  },

							  [this, &val](const Number& num)
							  {
								  val = num;
							  } },
		*factor.expr_or_num);
	return val;
}

int64_t aec::Evaluator::eval(const Number& num)
{
	return num;
}

int64_t Evaluator::result() const
{
	return res;
}

void aec::Printer::run(const AbstractSyntaxTree& tree)
{
	AsSuper::run(tree);
	if (buf.ends_with(' '))
		buf.pop_back();
}

void Printer::operator()(const Expr& expr)
{
	self_visit(*expr.term);
	for (auto& term : expr.follows)
		self_visit(*term);
}

void Printer::operator()(const Term& term)
{
	self_visit(*term.factor);
	for (auto& factor : term.follows)
		self_visit(*factor);
}

void Printer::operator()(const Factor& factor)
{
	std::visit(OverloadedPass{ [this](const Expr& expr)
							  {
								  record("(");
								  self_visit(expr);
								  record(")");
							  },

							  [this](const Number& num)
							  {
								  self_visit(num);
							  } },
		*factor.expr_or_num);
}

void Printer::operator()(const UnaryExpr<Term>& unary_expr)
{
	record(unary_expr.op);
	self_visit(unary_expr.operand);
}

void Printer::operator()(const UnaryExpr<Factor>& unary_expr)
{
	record(unary_expr.op);
	self_visit(unary_expr.operand);
}

void Printer::operator()(const Number& num)
{
	record(std::to_string(num));
}

void Printer::record(const aec::Operator& op)
{
	using aec::Operator;
	std::string opstr;
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

void Printer::record(const string& info)
{
	this->buf.append(info).push_back(' ');
}

string Printer::result() const
{
	return buf;
}

void Prosthaphaeresis::operator()(Expr& expr)
{
	self_visit(*expr.term);
	for (auto& term : expr.follows)
		self_visit(term->operand);
}

void Prosthaphaeresis::operator()(Term& term)
{
	auto is_number = [](const Factor& node)
	{
		return std::holds_alternative<Number>(*node.expr_or_num);
	};

	/// only process Term node like "a * b" and requires both a, b are 'Number'
	/// case like "a * b * c" or "a * b * c * ...." is not included 
	if (auto& left = term.factor; is_number(*left))
		if (term.follows.size() == 1)
			if (auto& right = term.follows.front();
				right->op == Operator::Mul && is_number(right->operand))
	{
		auto loprand = std::get<Number>(*left->expr_or_num);
		auto roprand = std::get<Number>(*right->operand.expr_or_num);

		/// init new node
		decltype(Term::follows) follow;
		for (auto i = 0; i < roprand - 1; ++i)
			follow.push_back(std::make_unique<UnaryExpr<Factor>>(UnaryExpr<Factor>{
				.op = Operator::Add, 
				.operand = std::make_unique<Factor::Value>(loprand)
			}));

		term.factor = std::make_unique<Factor>(Factor{
			.expr_or_num = std::make_unique<Factor::Value>(Expr{
				.term = std::make_unique<Term>(Term{
					.factor = std::make_unique<Factor>(Factor{
						.expr_or_num = std::make_unique<Factor::Value>(loprand)}),
						.follows = std::move(follow)}),
				.follows = {}})
		});

		term.follows.clear();
	}
}

void Prosthaphaeresis::operator()(Factor& fac)
{
	AsSuper::operator()(fac);
}

