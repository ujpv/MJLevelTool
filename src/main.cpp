#include <iostream>
#include <map>
#include <Plist.hpp>

#include "Config/ToolConfig.h"
#include "LevelObject/LevelObject.h"
#include "stdexcept"

using namespace std;

const std::string path = "d:\\develop\\MJLevelTool\\data\\level_1_29.plist ";
//const std::string gConfigPath = "d:\\develop\\tool\\bin\\params_test.json";
const std::string gConfigPath = "d:\\develop\\tool\\bin\\params.json";

inline void ExitMassage()
{
//  std::cout << "\nPress eny key for exit...\n";
//  std::getchar();
}

int main()
{
  MJLevelObject level;
  try {
    level.initWithDictionary(path);
  }
  catch(exception e)
  {
    std::cerr << (std::string("\nError: ") + e.what()) << std::endl;
    ExitMassage();
    return 1;
  }

  ExitMassage();
  return 0;
}
