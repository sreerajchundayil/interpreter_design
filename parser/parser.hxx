#ifndef PARSER_HXX
#define PARSER_HXX
#include "astClasses.hxx"
#include <string>
#include <vector>

class Parser
{
  public:
    std::vector<Token> mTokens;
    int current = 0;

    Parser(std::vector<Token> tokens);
    Expr* expression();
    Expr* equality();
    std::vector<Statement*> parse();
    Statement* statement();
    Statement* expressionStatement();
    Statement* printStatement();
    Token peek();
    Token previous();
    bool IsAtEnd();
    bool check(TokenType type);
    Token advance();
    template<typename... Args>
    bool match(Args... types);
    Expr* comparison();
    Expr* term();
    Expr* unary();
    Expr* primary();
    Token consume(TokenType type, std::string message);
    void errorMessage(Token token, std::string message);
    Expr* factor();
};

#endif
