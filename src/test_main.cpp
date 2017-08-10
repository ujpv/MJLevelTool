#include <iostream>
#include <map>
#include <stdexcept>

#include <Plist.hpp>
#include "Config/ToolConfig.h"
#include "LevelObject/LevelObject.h"
#include "Exceptions/Exceptions.h"

//const std::string LEVEL_PATH = "/home/chainic-vina/develop/MJLevelTool/data/levels/";
//const std::string TESTS_PATH = "/home/chainic-vina/develop/MJLevelTool/data/tests/";
const std::string LEVEL_PATH = "d:\\develop\\MJLevelTool\\data\\levels\\";
const std::string TESTS_PATH = "d:\\develop\\MJLevelTool\\data\\tests\\";
const std::string LEVEL_PREFIX = "level_test_";

static const char * kLevelName = "level_name";
static const char * kSeed = "seed";
static const char * kId = "id";
static const char * kType = "type";
static const char * kNeighbors = "neighbors";
static const char * kTop = "top";
static const char * kBottom = "bottom";
static const char * kRigth = "right";
static const char * kLeft = "left";
static const char * kCFG = "CFG";
static const char * kChips = "chips";

static const int FILE_COUNT = 111;
static const size_t REPEAT_COUNT = 10000;

inline void ExitMassage()
{
  std::cout << "\nPress any key for exit...\n";
  std::getchar();
}

int main()
{
  for (int i = 0; i < FILE_COUNT; ++i)
  {
    std::string testFileName = TESTS_PATH + LEVEL_PREFIX + std::to_string(i) + ".json";

    std::cout << "Running test " << testFileName << "...";

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

      cfg.Load(root[kCFG], "unkown");

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

    MJLevelObject level;
    double result = 0;
    try
    {
      level.InitWithDictionary(LEVEL_PATH + plistFileName);
      level.SetCFG(cfg);
      level.SetRepeatTypeTimes(1);
      level.BuildWithSeed(seed);
      result = level.PlayRandomNTimes(REPEAT_COUNT);
    }
    catch (std::exception & e)
    {
      std::cout << "\nCan't build level: " << plistFileName << ". Reason: " << e.what() << '\n';
      continue;
    }

    level.RestoreFromCache();

    std::set<std::pair<std::string, std::string>> chipTypesSet;
    std::map<std::string, std::vector<std::set<std::string>>> neighborsMap;

    const std::vector<MJChip> & chips = level.GetChips();
    for (const MJChip & chip: chips)
      chipTypesSet.insert({chip.GetID(), chip.GetTypeValue()});
      
    for (const MJChip & chip: chips)
    {
      const std::string & name = chip.GetID();
      neighborsMap[name] = std::vector<std::set<std::string>>(4);

      for (const MJChip * chip1: chip.GetNeighbors(Top))
        neighborsMap[name][Top].insert(chip1->GetID());

      for (const MJChip * chip1: chip.GetNeighbors(Bottom))
        neighborsMap[name][Bottom].insert(chip1->GetID());

      for (const MJChip * chip1: chip.GetNeighbors(Left))
        neighborsMap[name][Left].insert(chip1->GetID());

      for (const MJChip * chip1: chip.GetNeighbors(Right))
        neighborsMap[name][Right].insert(chip1->GetID());
    }

    bool isFailed = false;

    for (auto & pair: neighborsMap)
    {
      auto & test = neighborsMapTest[pair.first];
      auto & current = pair.second;
      if (test != current)
      {
        std::cout << "\nNeighbors error for chip: " << pair.first << '\n';
        isFailed = true;
      }
    }

    if (chipTypesSet.size() != chipTypesSetTest.size())
    {
      isFailed = true;
      std::cout << "\nchipTypesSetTest size error\n";
    }

    for (auto & chipType: chipTypesSet)
    {
      if (!chipTypesSetTest.count(chipType))
      {
        std::cout << "\nchipTypesSet error:" << chipType.first << ", " << chipType.second<< '\n';
        isFailed = true;
      }
    }

    for (auto & chipType: chipTypesSetTest)
    {
      if (!chipTypesSet.count(chipType))
      {
        std::cout << "\nchipTypesSetTest error:" << chipType.first << ", " << chipType.second<< '\n';
        isFailed = true;
      }
    }

    std::cout << "\rRunning test " << testFileName << (isFailed ? "...Failed\n" : "...Ok\n");
    std::cout << "Level: " << plistFileName << ", Seed: " << seed << ", Result: " << result << '\n';
  }

  ExitMassage();

  return 0;
}
