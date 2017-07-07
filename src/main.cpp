#include <iostream>
#include <map>
#include <Plist.hpp>

#include "Config/ToolConfig.h"
#include "LevelObject/LevelObject.h"
#include <stdexcept>

using namespace std;

const std::string path = "d:\\develop\\MJLevelTool\\data\\levels\\level_1_29.plist ";
//const std::string gConfigPath = "d:\\develop\\tool\\bin\\params_test.json";
const std::string gConfigPath = "d:\\develop\\MJLevelTool\\data\\params_new.json ";

inline void ExitMassage()
{
//  std::cout << "\nPress eny key for exit...\n";
//  std::getchar();
}

int main()
{
  CToolConfig & config = CToolConfig::Instance();
  std::cout << "Loadin config...";
  try
  {
    config.Init(gConfigPath);
  }
  catch (std::exception & e)
  {
    std::cout << "\n Can't read config. Reason: " << e.what();
    ExitMassage();
    return 0;
  }
  std::cout << "\rLoadin config...OK\n";

  std::cout << "Loadin level...";
  MJLevelObject level;
  try {
    level.InitWithDictionary(path);
    level.SetCFG(config.GetCGF("25"));
//    level.SetCFG(SCFG{
//          9, //int m_iFirstGroupDigitChipsNumber;
//          9, //int m_iSecondGroupDigitChipsNumber;
//          9, //int m_iThirdGroupDigitChipsNumber;
//          0, //int m_iMysticGroupChipsNumber;
//          3, //int m_iDragonChipGroupNumber;
//          4, //int m_iFlowerChipGroupNumber;
//          4, //int m_iSeasonChipGroupNumber;
//          4  //int m_iWindChipGroupNumber;
//       });
    level.BuildWithSeed(284);
  }
  catch(exception & e)
  {
    std::cerr << (std::string("Error: ") + e.what()) << std::endl;
    ExitMassage();
    return 1;
  }
  std::cout << "\rLoadin level...OK\n";

  ExitMassage();
  return 0;
}
