#include <iostream>
#include <fstream>
#include <list>
#include <string>
#include <vector>

using namespace std;

std::string trim(const std::string& text)
{
  std::string temp = text;
  int startIndex = 0;
  int stopIndex = text.size() - 1;

  {
    while(text[startIndex] == ' ' && (startIndex < stopIndex))
      startIndex++;
    while(text[stopIndex] == ' ' && (stopIndex > startIndex))
      stopIndex--;
  }
  return text.substr(startIndex, stopIndex - startIndex+1);
}

std::vector<std::string> SplitLineWithDelim(std::string text, char delim)
{
  std::vector<std::string> myVector;
  size_t pos = 0;
  size_t start = 0;
  while(pos < text.size())
  {
    if(text[pos] == delim)
    {
      myVector.push_back(trim(text.substr(start, pos-start)));
      start = pos+1;
    } 
    pos++;
  }
  myVector.push_back(trim(text.substr(start, pos)));
  return myVector;
}

class AstTemplateGenerator 
{
  std::ofstream ofs;
  public:
    void DefineTypes(std::string baseName, std::string className, std::string fieldList)
    {
      auto typeAndVariable = SplitLineWithDelim(fieldList, ',');
      ofs << "class " + className + " : public " + baseName +
          "\n{\n"
          + "  private:\n";
      for(auto elem : typeAndVariable)
      {
        auto splitTypeAndVariable = SplitLineWithDelim(elem, ' ');
        ofs << "    " << splitTypeAndVariable[0] << "& " << splitTypeAndVariable[1] << ";\n";
      }
      ofs << "  public:\n    " + className + "(";
      for(size_t i = 0; i < typeAndVariable.size(); i++)
      {
        auto splitTypeAndVariable = SplitLineWithDelim(typeAndVariable[i], ' ');
        if( i == typeAndVariable.size() - 1)
          ofs << splitTypeAndVariable[0] << "& " << splitTypeAndVariable[1];
        else
          ofs << splitTypeAndVariable[0] << "& " << splitTypeAndVariable[1] << ", ";
      }
      ofs << "):\n      ";
      for(size_t i = 0; i < typeAndVariable.size(); i++)
      {
        const auto& elem = typeAndVariable[i];
        if(i == typeAndVariable.size()-1)
          ofs << SplitLineWithDelim(elem, ' ')[1] + "(" + SplitLineWithDelim(elem, ' ')[1] + ")\n";
        else
          ofs << SplitLineWithDelim(elem, ' ')[1] + "(" + SplitLineWithDelim(elem, ' ')[1] + "), ";
      }
      ofs<< "    {\n";

      ofs << "    }\n";

      ofs <<  "    void Accept(Visitor* visitor)\n";
      ofs <<  "    {\n";
      ofs <<  "       visitor->Visit" << className << "(this);\n";
      ofs <<  "    }\n";

      ofs << "};\n\n";


      
    }
    void GenerateTemplate()
    {
      ofs.open("astClasses.cpp");  
      std::list<std::string> types =
      {
        "Binary   : Expr left, Token operation, Expr right",
        "Grouping : Expr expression",
        "Literal  : std::string value",
        "Unary    : Token operation, Expr right"
      };

      ofs << "#include \"lexer.hxx\"\n";
      ofs << "#include <string>\n";
      std::vector<std::string> classNames;
      std::vector<std::string> fieldLists;
      for(const std::string& type : types)
      {
        auto pos = type.find(':');
        classNames.push_back(trim(type.substr(0,pos)));
        fieldLists.push_back(trim(type.substr(pos+1)));
      }
     
      //Declaring concrete classes 
      for(const auto& className : classNames)
        ofs << "class " + className + ";\n"; 
      ofs << "\n";
      ofs << "class Visitor{\n"
          << "  public:\n";
      for(const auto& className : classNames)
      {
        ofs << "    virtual void Visit" << className 
          << "(const " << className << " *element) const = 0;\n";
      }

      ofs << "};\n\n";

      ofs << "class ConcreteVisit1" << ": public Visitor\n";
      ofs << "{\n";
      ofs << "  public:\n";

      for(const auto& className: classNames)
      {
        ofs << "    void Visit" << className << "(const " << className << " *elem)\n";
        ofs << "    {\n";
        ofs << "    }\n";
      }
      ofs << "};\n";


      std::string baseName = "Expr";

      ofs << "class " + baseName + "\n{\n"; 
      ofs << "  public:\n";
      ofs << "    virtual ~" << baseName << "(){}\n";
      ofs << "    virtual void Accept(Visitor *visitor) const = 0;\n";
      ofs << "};\n\n";

      //Create Sub Classes ofAST
      for(const std::string& type : types)
      {
        auto pos = type.find(':');
        auto className = trim(type.substr(0,pos));
        auto fieldList = trim(type.substr(pos+1));
        DefineTypes(baseName, className, fieldList);
      }

    }
};

int main(int argc, char* argv[])
{
  AstTemplateGenerator generator;
  generator.GenerateTemplate();
  return 0;
}
