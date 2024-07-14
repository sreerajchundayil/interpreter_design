#ifndef ASTCLASSES_HXX
#define ASTCLASSES_HXX
#include <string>
#include <utility>
#include <iostream>



class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor;


class Expr
{
  public:
    virtual ~Expr(){}
    virtual Expr* Accept(Visitor *visitor) const = 0;
};

class Visitor
{
  public:
    virtual Expr* VisitBinary(const Binary *element) = 0;
    virtual Expr* VisitGrouping(const Grouping *element) = 0;
    virtual Expr* VisitLiteral(const Literal *element) = 0;
    virtual Expr* VisitUnary(const Unary *element) = 0;
};

class Binary : public Expr
{
  public:
    Expr* left;
    Token operation;
    Expr* right;
  public:
    Binary(Expr* left, Token operation, Expr* right):
      left(left), operation(operation), right(right)
    {
    }
    virtual Expr* Accept(Visitor* visitor)const override
    {
       return visitor->VisitBinary(this);
    }
};

class Grouping : public Expr
{
  public:
    Expr* expression;
    TokenType type;
  public:
    Grouping(Expr* expression):
      expression(expression)
    {
    }
    Expr* Accept(Visitor* visitor)const override
    {
       return visitor->VisitGrouping(this);
    }
};

class Literal : public Expr
{
  public:
    std::string value;
    TokenType type;
  public:
    Literal(std::string value, TokenType type):
      value(value), type(type)
    {
    }
    Expr* Accept(Visitor* visitor)const override
    {
       return visitor->VisitLiteral(this);
    }
};

class Unary : public Expr
{
  public:
    Token operation; // -, ~ etc..
    Expr* right;
  public:
    Unary(Token operation, Expr* right):
      operation(operation), right(right)
    {
    }
    Expr* Accept(Visitor* visitor)const override
    {
       return visitor->VisitUnary(this);
    }
};

class Interpreter: public Visitor
{
  public:
    Expr* interpret(Expr* expression)
    {
      return evaluate(expression);
    }
    Expr* VisitLiteral(const Literal* expr) override
    {
      return new Literal{expr->value, expr->type};
    }

    Expr* VisitGrouping(const Grouping* expr) override
    {
      return evaluate(expr->expression);
    }

    Expr* evaluate(Expr* expr)
    {
      return expr->Accept(this);
    }

    int IsTruthy(std::string object, TokenType type)
    {
      if(type == TokenType::NUMBER)
      {
          int number = std::stoi(object);
          return number;
      }
      return 0;
    }

    Expr* VisitUnary(const Unary* expr) override
    {
      Literal* right = (Literal*)evaluate(expr->right);
      if(right->type != TokenType::NUMBER)
      {
        
        std::cout << "Expected integer: "<< std::endl;
        exit(-1);
      }

      switch(expr->operation.type)
      {
        case TokenType::MINUS:
          return new Literal{std::to_string(-1*std::stoi(right->value)), TokenType::NUMBER};
        case TokenType::BANG:
          return new Literal(std::to_string(!IsTruthy(right->value, right->type)), TokenType::NUMBER);
      }
      return nullptr;
    }


    Expr* VisitBinary(const Binary* expr) override
    {
      Literal* left = (Literal*)evaluate(expr->left);
      Literal* right = (Literal*)evaluate(expr->right);

      if(left->type != right->type)
      {
        std::cout << "Different value type" << std::endl;
        exit(-1);
      }

      switch(expr->operation.type)
      {
        case MINUS:
          return new Literal{std::to_string(std::stoi(left->value) - std::stoi(right->value)), TokenType::NUMBER};
        case SLASH:
          return new Literal{std::to_string(std::stoi(left->value) / std::stoi(right->value)), TokenType::NUMBER};
        case STAR:
          return new Literal{std::to_string(std::stoi(left->value) * std::stoi(right->value)), TokenType::NUMBER};
        case PLUS:
        {
          if (left->type == TokenType::STRING)
          {
            return new Literal{left->value + right->value, TokenType::STRING};
          }
          else
          {
            return new Literal(std::to_string(std::stoi(left->value) + std::stoi(right->value)), TokenType::NUMBER);
          }
        }
      }

      std::cout << "Unexpected error while binary operation." << std::endl;
      exit(-1);
      return new Literal("", TokenType::EOFF);
      
    }
};


#endif
