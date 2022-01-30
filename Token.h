#pragma once
#include <variant>

namespace aec
{
    using std::variant;

    /// (  )
    enum class Delim
    {
        LeftParent,
        RightParent,
    };

    /// +  -  *  /
    enum class Operator
    {
        Add,
        Sub,
        Mul,
        Div,
    };

    /// positive interger
    using Number = size_t;

    template <typename T>
    concept OneOfTokenType = requires(T _)
    {
        std::is_same<T, Number>() 
        || std::is_same<T, Delim>() 
        || std::is_same<T, Operator>();
    };

    class Token
    {
    public:
        using TokenType = variant<Number, Operator, Delim>;

    public:
        static const Token None;

    public:
        Token() = delete;
        Token(const Token &) = default;
        Token(Token &&) = default;
        ~Token() = default;
        Token &operator=(const Token &) = default;

    public:
        Token(Delim del);
        Token(Operator op);
        Token(Number num);

    public:
        template <OneOfTokenType T>
        constexpr bool is() const noexcept
        {
            return std::holds_alternative<T>(_inner);
        }

        template <OneOfTokenType T>
        T get_unchecked() const
        {
            return std::get<T>(_inner);
        }

        bool operator==(const Token &token) const
        {
            return _inner == token._inner;
        }

    private:
        TokenType _inner;
    };

} // end namespace aec
