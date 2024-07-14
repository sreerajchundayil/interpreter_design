#include <string>
#include <map>
#include "lexer.hxx"
#include "token.hxx"
#include <iostream>
#include <cctype>

bool hasError= false;

std::vector<std::string> TokenTypeName = 
{
  //single-character-token
  "LEFT_PAREN",
  "RIGHT_PAREN",
  "LEFT_BRACE",
  "RIGHT_BRACE",
  "COMMA",
  "DOT",
  "MINUS",
  "PLUS",
  "SEMICOLON",
  "SLASH",
  "STAR",

  //
  "BANG",
  "BANG_EQUAL",
  "EQUAL",
  "EQUAL_EQUAL",
  "GREATER",
  "GREATER_EQUAL",
  "LESS",
  "LESS_EQUAL",

  //literal
  "IDENTIFIER",
  "STRING",
  "NUMBER",

  //Keywords
  "AND",
  "CLASS",
  "ELSE",
  "FALSE",
  "FUNC",
  "FOR",
  "IF",
  "NIL",
  "OR",
  "PRINT",
  "RETURN",
  "SUPER",
  "THIS",
  "TRUE",
  "VAR",
  "WHILE",
  "EOFF"
};

std::map<std::string, TokenType> keywords =
{
    {"and",    AND},
    {"class",  CLASS},
    {"else",   ELSE},
    {"false",  FALSE},
    {"for",    FOR},
    {"func",    FUNC},
    {"if",     IF},
    {"nil",    NIL},
    {"or",     OR},
    {"print",  PRINT},
    {"return", RETURN},
    {"super",  SUPER},
    {"this",   THIS},
    {"true",   TRUE},
    {"var",    VAR},
    {"while",  WHILE}
};

Lexer::Lexer(const std::string& str):mCommand(str)
{
}

std::vector<Token> Lexer::Tokenize()
{
  Scanner scanner(mCommand);
  std::vector<Token> tokens = scanner.ScanTokens();
  scanner.PrintTokens();
  return tokens;
}

bool Scanner::IsAtEnd()//Consumed all characters.
{
  return current >= source.size();
}

char Scanner::advance() 
{
  column++;
  return source[current++];
}
char Scanner::PeekNext()
{
  if (current + 1 >= source.length()) return '\0';
  return source[current + 1];
}
Scanner::Scanner(std::string source)
{
  this->source = source;
}

bool Scanner::match(char expected)
{
  if(IsAtEnd())
    return false;
  if(source[current] != expected)
    return false;
  column++;
  current++;
  return true;
}

char Scanner::peek()
{
  if(IsAtEnd()) return '\0';
  return source[current];
}

void Scanner::CreateString()
{
  while(peek() != '"' && !IsAtEnd())
  {
    if(peek() == '\n')
      line++;
    advance();
  }

  if(IsAtEnd())
  {
    std::cout << "Error at line number " << line << std::endl;
    return;
  }

  advance(); //Consume the closing character "

  //Skip start and end " and create the string
  std::string value = source.substr(start+1, current-start-2);
  addToken(STRING, value);
}

void Scanner::CreateNumber()
{
  while(isdigit(peek()))
    advance();
  if(!isspace(peek()))
  {
      std::cout << "Invalid number : error at " << line << " at column " << column << std::endl;
      exit(-1);
  }
  
  std::string substring1 = source.substr(start, current-start);
  addToken(NUMBER, substring1);
}

void Scanner::identifier()
{
  while(isalpha(peek()))
    advance();
  std::string text = source.substr(start, current-start);
  auto itr = keywords.find(text);
  if(itr == keywords.end())
    addToken(IDENTIFIER);
  else
  {
    addToken(itr->second);
  }
}

void Scanner::ScanToken()
{
  char c = source[current++]; //the next character
  column++;
  switch(c)
  {
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(MINUS); break;
    case '+': addToken(PLUS); break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break;
    case '!':
      addToken(match('=') ? BANG_EQUAL : BANG);
      break;
    case '=':
      addToken(match('=') ? EQUAL_EQUAL : EQUAL);
      break;
    case '<':
      addToken(match('=') ? LESS_EQUAL : LESS);
      break;
    case '>':
      addToken(match('=') ? GREATER_EQUAL : GREATER);
      break;

    case '/':
      if(match('/'))
      {
        while(peek() != '\n' && !IsAtEnd()) advance();
      }
      else
      {
        addToken(SLASH);
      }
      break;

     case ' ':
     case '\r':
     case '\t':
      break;
     case '\n':
      column = 0;
      line++;
      break;

     case '"':
      CreateString();
      break;

    default:
    {
      if(isdigit(c))
      {
        CreateNumber();
      }
      else if(isalpha(c))
      {
        identifier();
      }
      else
      {
        std::cout << "Invalid character:Error at line number " << line << std::endl;
      }
      break;
    }
  }
}

void Scanner::addToken(TokenType type)
{
  addToken(type, "");
}

void Scanner::addToken(TokenType type, std::string literal)
{
  std::string text = source.substr(start, current-start);
  tokens.push_back(Token(type, text, literal, line));
}


std::vector<Token> Scanner::ScanTokens()
{
  while(!IsAtEnd())
  {
    start = current;
    ScanToken();
  }
  tokens.push_back(Token(TokenType(EOFF), "", "", line));
  return tokens;
}

void Scanner::PrintTokens()
{
  for(auto& elem: tokens)
  {
    std::cout << elem.toString(); 
  }
}


Token::Token(TokenType type, std::string lexeme, std::string literal, int line)
{
  this->type = type;
  this->lexeme = lexeme;
  this->literal = literal;
  this->line = line;
}

std::string Token::toString()
{
  return "Type : " + TokenTypeName[int(type)] + " ,Lexeme : " + lexeme
         + " ,value : " + literal + " ,line:" + std::to_string(line) + "\n";
}


