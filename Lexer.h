#pragma once

#include <string>
#include <string_view>
#include <vector>
#include <optional>

#include "Token.h"

namespace aec
{
    using std::optional;
    using std::string;
    using std::vector;

    class Scanner
    {
    public:
        using Output = char;
        using InputStream = string;
        using InnerBuffer = vector<char>;

    public:
        constexpr static Output eof = 0x00;

    public:
        Scanner() = delete;
        Scanner(const Scanner &) = default;
        ~Scanner() = default;

    public:
        Scanner(const InputStream &text);

    public:
        Output peek() const noexcept;
        Output next() noexcept;
        bool is_eof() const noexcept;

    private:
        InnerBuffer _buf;
        size_t _cursor;
    };

    using std::string_view;

    class Lexer
    {
    public:
        Lexer() = delete;
        Lexer(const Lexer &) = default;
        ~Lexer() = default;

    public:
        Lexer(const string &text);

    public:
        Token peek() noexcept;
        Token next();
        bool is_eof() const noexcept;

    protected:
        // error handler
        void unexpect(string_view &&error) const;

    private:
        Token _iter_next();

        // each case
        Token _eat_number();

    private:
        Scanner _zin;
        string _buf;
        optional<Token> _now;
    };

} // end namespace aec