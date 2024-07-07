#ifndef ASTCLASSES_HXX
#define ASTCLASSES_HXX
#include "../lexer/lexer.hxx"
#include <string>
class Binary;
class Grouping;
class Literal;
class Unary;

class Visitor{
  public:
    virtual std::string VisitBinary(const Binary *element) const = 0;
    virtual std::string VisitGrouping(const Grouping *element) const = 0;
    virtual std::string VisitLiteral(const Literal *element) const = 0;
    virtual std::string VisitUnary(const Unary *element) const = 0;
};

class Expr
{
  public:
    virtual ~Expr(){}
    virtual std::string Accept(Visitor *visitor) const = 0;
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
    virtual std::string Accept(Visitor* visitor)const override
    {
       return visitor->VisitBinary(this);
    }
};

class Grouping : public Expr
{
  public:
    Expr* expression;
  public:
    Grouping(Expr* expression):
      expression(expression)
    {
    }
    std::string Accept(Visitor* visitor)const override
    {
       return visitor->VisitGrouping(this);
    }
};

class Literal : public Expr
{
  public:
    std::string valueStr;
    bool valueBool;
  public:
    Literal(std::string value):
      valueStr(value)
    {
    }
    Literal(bool value):
      valueBool(value)
    {
    }
    std::string Accept(Visitor* visitor)const override
    {
       return visitor->VisitLiteral(this);
    }
};

class Unary : public Expr
{
  public:
    Token operation;
    Expr* right;
  public:
    Unary(Token operation, Expr* right):
      operation(operation), right(right)
    {
    }
    std::string Accept(Visitor* visitor)const override
    {
       return visitor->VisitUnary(this);
    }
};


#endif
