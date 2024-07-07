#include <iostream>
#include <fstream>
#include <sstream>
#include <list>
#include <string>
#include <map>
#include "lexer/lexer.hxx"

using namespace std;

extern bool hasError;

void runLine(const std::string& str)
{
  //std::cout << str;
  Lexer lex(str);
  lex.Tokenize();
}

void run(char* file)
{
  std::ifstream ifs(file);
  std::stringstream buffer;
  buffer << ifs.rdbuf();
  runLine(buffer.str());
  if(hasError)
    exit(-1);
}

void runPrompt()
{
  for(;;)
  {
    std::string command;
    std::cout << "< ";
    std::getline(std::cin, command);
    runLine(command);
    hasError = false;
    cout << "\n";
  }
}

int main(int argc, char* argv[])
{
  if(argc >= 3)
    return -1;
  else if(argc == 2)
  {
    //If a file is given interpret it.
    run(argv[1]);
  }
  else
  {
    //If no file is given. Start a prompt where user can enter commands.
    runPrompt();
  }

  return 0;
}

