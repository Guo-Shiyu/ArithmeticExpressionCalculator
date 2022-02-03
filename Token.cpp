#include "Token.h"

namespace aec
{
	Token Token::eof = { std::numeric_limits<Number>::max() };

	Token::Token(Delim def) : _inner(def) {}

	Token::Token(Operator op) : _inner(op) {}

	Token::Token(Number number) : _inner(number) {}

	bool Token::operator==(const Token& token) const
	{
		return _inner == token._inner;
	}

	string Token::to_string() const
	{
		if (is<Delim>())
		{
			switch (get_unchecked<Delim>())
			{
			case Delim::LeftParent:
				return { '(' };
			case Delim::RightParent:
				return { ')' };

			default: /// unreachable code
				return {};
				break;
			}
		}
		else if (is<Operator>())
		{
			switch (get_unchecked<Operator>())
			{
			case Operator::Add:
				return { '+' };
			case Operator::Sub:
				return { '-' };
			case Operator::Mul:
				return { '*' };
			case Operator::Div:
				return { '/' };

			default: /// unreachable code
				return {};
				break;
			}
		}
		else
		{
			if (auto inner = get_unchecked<Number>();
				inner == std::get<Number>(Token::eof._inner))
				return { "EOF" };
			else 
				return std::to_string(inner);
		}
	}
}