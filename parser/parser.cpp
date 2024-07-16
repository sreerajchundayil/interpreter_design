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
  return peek().type == EOFF;
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
    return new Literal("1",TokenType::NUMBER);
  if (match(TRUE))
    return new Literal("0",TokenType::NUMBER);

  if (match(NUMBER))
  {
    return new Literal(previous().literal, TokenType::NUMBER);
  }
  if (match(STRING))
  {
    return new Literal(previous().literal, TokenType::STRING);
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

std::vector<Statement*> Parser::parse()
{
  std::vector<Statement*> statements;
  while(!IsAtEnd())
    statements.push_back(statement());
  return statements;
}

Statement* Parser::statement()
{
  if(match(PRINT))
    return printStatement();
  return expressionStatement();
}

Statement* Parser::printStatement()
{
  Expr* value = expression();
  consume(SEMICOLON, "Expect';' after value.");
  return new Print(value);
}

Statement* Parser::expressionStatement()
{
  Expr* value = expression();
  consume(SEMICOLON, "Expect';' after expression.");
  return new Expression(value);
}




