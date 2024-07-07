#ifndef LEXER_HXX
#define LEXER_HXX

#include <string>
#include <list>
#include "token.hxx"

class Token
{
  public:
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;
  public:
    Token(TokenType type, std::string lexeme, std::string literal, int line);
    std::string toString();
};

class Lexer
{
    const std::string& mCommand;
  public:
    Lexer(const std::string& str);
    std::list<Token> Tokenize();
};

class Scanner
{
  public:
    std::string source;
    std::list<Token> tokens;

    int start = 0; //First character of a lexeme getting processed.
    int current = 0; //Current character being processed.
    int line = 1;
    int column = 1;

    bool IsAtEnd();//Consumed all characters.

  public:
    char advance() ;
    char PeekNext();
    Scanner(std::string source);
    bool match(char expected);
    char peek();
    void CreateString();
    void CreateNumber();
    void identifier();
    void ScanToken();
    void addToken(TokenType type);
    void addToken(TokenType type, std::string literal);
    std::list<Token> ScanTokens();
    void PrintTokens();
};

#endif
