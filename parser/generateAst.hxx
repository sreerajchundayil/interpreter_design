#ifndef GENERATEAST__HXX
#define GENERATEAST__HXX
#include <string>
#include <fstream>

class AstTemplateGenerator 
{
  std::ofstream ofs;
  public:
    void DefineTypes(std::string baseName, std::string className, std::string fieldList);
    void GenerateTemplate();
};
#endif
