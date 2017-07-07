#include <iostream>
#include <map>
#include <filesystem>
#include <stdexcept>

#include <Plist.hpp>

#include "Config/ToolConfig.h"
#include "LevelObject/LevelObject.h"
#include "Exceptions/Exceptions.h"

const std::string LEVEL_PATH = "d:\\develop\\MJLevelTool\\data\\levels\\";
const std::string TESTS_PATH = "d:\\develop\\MJLevelTool\\data\\tests\\";
const std::string LEVEL_PREFIX = "level_test_";

static const char * kLevelName = "level_name";
static const char * kSeed = "seed";
static const char * kRandom = "random";
static const char * kId = "id";
static const char * kType = "type";
static const char * kNeighbors = "neighbors";
static const char * kTop = "top";
static const char * kBottom = "bottom";
static const char * kRigth = "right";
static const char * kLeft = "left";

static const char * kDigitChipFirstGroupNumber = "DigitChipFirstGroupNumber";
static const char * kDigitChipSecondGroupNumber = "DigitChipSecondGroupNumber";
static const char * kDigitChipThirdGroupNumber = "DigitChipThirdGroupNumber";
static const char * kDragonChipGroupNumber = "DragonChipGroupNumber";
static const char * kFlowerChipGroupNumber = "FlowerChipGroupNumber";
static const char * kMysticChipGroupNumber = "MysticChipGroupNumber";
static const char * kSeasonChipGroupNumber = "SeasonChipGroupNumber";
static const char * kWindChipGroupNumber = "WindChipGroupNumber";

const int FILE_COUNT = 1;

inline void ExitMassage()
{
//  std::cout << "\nPress eny key for exit...\n";
//  std::getchar();
}

int main()
{
  for (int i = 0; i < FILE_COUNT; ++i)
  {
    std::string testFileName = TESTS_PATH + LEVEL_PREFIX + std::to_string(i) + ".json";

    std::string plistFileName;
    int seed;
    std::set<std::pair<std::string, std::string>> chipTypesSet;
    std::map<std::string, std::vector<std::set<std::string>>> neighborsSet;

    try
    {
      Json::Value root;
      Json::Reader reader;
      std::ifstream fileStream(testFileName, std::ifstream::binary);
      if (!reader.parse(fileStream, root, false) || root.isNull())
        throw MJToolException("Can't parse config " + testFileName);

      plistFileName = root.get(kLevelName, "").asString();
      if (plistFileName.empty())
        throw MJToolException("Empty filename");

      seed = root.get(kSeed, -1).asInt();
      if (seed < 0)
        throw MJToolException("Invalid seed");


    }
    catch (std::exception & e)
    {
      std::cout << "\nCan't read test. Reason: " << e.what() << '\n';
      continue;
    }


  }
  CToolConfig & config = CToolConfig::Instance();
////  std::cout << "Loadin config...";
//  try
//  {
//    config.Init(gConfigPath);
//  }
//  catch (std::exception & e)
//  {
//    std::cout << "\n Can't read config. Reason: " << e.what();
//    ExitMassage();
//    return 0;
//  }
//  std::cout << "\rLoadin config...OK\n";

//  std::cout << "Loadin level...";
//  MJLevelObject level;
//  try {
//    level.initWithDictionary(path);
//    level.SetCFG(config.GetCGF("25"));
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
//    level.BuildWithSeed(284);
//  }
//  catch(exception & e)
//  {
//    std::cerr << (std::string("Error: ") + e.what()) << std::endl;
//    ExitMassage();
//    return 1;
//  }
//  std::cout << "\rLoadin level...OK\n";

//  ExitMassage();
  return 0;
}
