#include "../lexer/lexer.hxx"
#include "parser.hxx"
#include <vector>
#include <iostream>


Parser::Parser(std::vector<Token> tokens)
{
  mTokens = tokens; 
}

Expr* Parser::expression()
{
  return equality();
}

Token Parser::peek()
{
  return mTokens[current];
}

Token Parser::previous()
{
  return mTokens[current-1];
}

bool Parser::IsAtEnd()
{
  return peek().type == EOF;
}

bool Parser::check(TokenType type)
{
  if(IsAtEnd())
    return false;
  return peek().type == type;
}

Token Parser::advance()
{
  if(!IsAtEnd())
    current++;
  return previous();
}

Expr* Parser::comparison()
{
  Expr* expr = term();
  while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL))
  {
    Token operation = previous();
    Expr* right = term();
    expr = new Binary(expr, operation, right);
  }
  return expr;
}

Expr* Parser::term()
{
  Expr* expr = factor();

  while (match(MINUS, PLUS))
  {
    Token operation = previous();
    Expr* right = factor();
    expr = new Binary(expr, operation, right);
  }

  return expr;
}

Expr* Parser::unary()
{
  if (match(BANG, MINUS))
  {
    Token operation = previous();
    Expr* right = unary();
    return new Unary(operation, right);
  }

  return primary();
}

Expr* Parser::primary()
{
  if (match(FALSE))
    return new Literal(false);
  if (match(TRUE))
    return new Literal(true);
  if (match(NIL))
    return new Literal(NIL);

  if (match(NUMBER, STRING))
  {
    return new Literal(previous().literal);
  }

  if (match(LEFT_PAREN))
  {
    Expr* expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return new Grouping(expr);
  }
  return nullptr;
}

Token Parser::consume(TokenType type, std::string message)
{
  if (check(type))
    return advance();
  errorMessage(peek(), message);
  exit(-1);
}

void Parser::errorMessage(Token token, std::string message)
{
  if(token.type == EOF)
  {
    std::cout << "Error at " << token.line << " at end " << message << std::endl;
  }
  else
  {
    std::cout << "Error at " << token.line << " at '" << token.lexeme << "' " << message << std::endl;
  }
}

Expr* Parser::factor()
{
  Expr* expr = unary();

  while (match(SLASH, STAR))
  {
    Token operation = previous();
    Expr* right = unary();
    expr = new Binary(expr, operation, right);
  }

  return expr;
}

template<typename... Args>
bool Parser::match(Args... types)
{
  for(auto type : {types...})
  {
    if(check(type))
    {
      advance();
      return true;
    }
  }
  return false;
}


Expr* Parser::equality()
{
  Expr* expr = comparison();
  while(match(BANG_EQUAL, EQUAL_EQUAL))
  {
    Token operation = previous();
    Expr* right = comparison();
    expr = new Binary(expr, operation, right);
  }
  return expr;
}


