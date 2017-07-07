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
static const char * kCFG = "CFG";
static const char * kChips = "chips";

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
    SCFG cfg;
    std::set<std::pair<std::string, std::string>> chipTypesSetTest;
    std::map<std::string, std::vector<std::set<std::string>>> neighborsMapTest;

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

      cfg.Load(root[kCFG]);

      Json::Value chipsTypes = root[kChips];

      if (chipsTypes.isNull() || !chipsTypes.isArray() || !chipsTypes.size())
        throw MJToolException(std::string("There are no correct ") + kChips + " section");

      for (Json::Value::const_iterator it = chipsTypes.begin(); it != chipsTypes.end(); ++it)
      {
        std::string id = it->get(kId, "").asString();
        std::string type = it->get(kType, "").asString();

        if (id.empty() || type.empty())
          throw MJToolException("Invalide chip type");

        chipTypesSetTest.insert({id, type});
      }

      Json::Value chipsNeighbors = root[kNeighbors];
      if (chipsNeighbors.isNull() || !chipsNeighbors.isObject() || !chipsNeighbors.size())
        throw MJToolException(std::string("There are no correct ") + kNeighbors + " section");

      for (const std::string & name: chipsNeighbors.getMemberNames())
      {
        Json::Value neighborsTop = chipsNeighbors[name][kTop];
        Json::Value neighborsBottom = chipsNeighbors[name][kBottom];
        Json::Value neighborsLeft = chipsNeighbors[name][kLeft];
        Json::Value neighborsRigth = chipsNeighbors[name][kRigth];

        neighborsMapTest[name] = std::vector<std::set<std::string>>(4);

        for (Json::Value::const_iterator it = neighborsTop.begin(); it != neighborsTop.end(); ++it)
          neighborsMapTest[name][Top].insert(it->asString());
        for (Json::Value::const_iterator it = neighborsBottom.begin(); it != neighborsBottom.end(); ++it)
          neighborsMapTest[name][Bottom].insert(it->asString());
        for (Json::Value::const_iterator it = neighborsLeft.begin(); it != neighborsLeft.end(); ++it)
          neighborsMapTest[name][Left].insert(it->asString());
        for (Json::Value::const_iterator it = neighborsRigth.begin(); it != neighborsRigth.end(); ++it)
          neighborsMapTest[name][Right].insert(it->asString());
      }
    }
    catch (std::exception & e)
    {
      std::cout << "\nCan't read test: " << testFileName << ". Reason: " << e.what() << '\n';
      continue;
    }

    try
    {
      MJLevelObject level;
      level.InitWithDictionary(LEVEL_PATH + plistFileName);
      level.SetCFG(cfg);
      level.BuildWithSeed(seed);
    }
    catch (std::exception & e)
    {
      std::cout << "\nCan't build level: " << plistFileName << ". Reason: " << e.what() << '\n';
      continue;
    }

    std::set<std::pair<std::string, std::string>> chipTypesSet;
    std::map<std::string, std::vector<std::set<std::string>>> neighborsMap;
  }

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
