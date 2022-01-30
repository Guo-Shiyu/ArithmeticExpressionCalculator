#include "Token.h"

namespace aec
{
    const Token Token::None = Token(0xFF);

    Token::Token(Delim def) : _inner(def) {}

    Token::Token(Operator op) : _inner(op) {}

    Token::Token(Number number) : _inner(number) {}
}