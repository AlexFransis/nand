#ifndef GUARD_jack_syntax_h
#define GUARD_jack_syntax_h


enum class KEYWORD {
        _CLASS,
        _CONSTRUCTOR,
        _FUNCTION,
        _METHOD,
        _FIELD,
        _STATIC,
        _VAR,
        _INT,
        _CHAR,
        _BOOLEAN,
        _VOID,
        _TRUE,
        _FALSE,
        _NULL,
        _THIS,
        _LET,
        _DO,
        _IF,
        _ELSE,
        _WHILE,
        _RETURN
};

enum class SYMBOL {
        _OPEN_CURLY_BRACE,
        _CLOSE_CURLY_BRACE,
        _OPEN_PAREN,
        _CLOSE_PAREN,
        _OPEN_BRACKET,
        _CLOSE_BRACKET,
        _PERIOD,
        _COMMA,
        _SEMI_COLON,
        _ADD,
        _SUB,
        _MUL,
        _DIV,
        _FORWARD_SLASH,
        _AND,
        _OR,
        _LESS_THAN,
        _GREATER_THAN,
        _EQUAL,
        _NOT
};

enum class TOKEN {
        KEYWORD,
        SYMBOL,
        INTEGER_CONST,
        STRING_CONST,
        IDENTIFIER,
        UNKNOWN
};

#endif
