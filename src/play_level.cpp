#include <iostream>

#include  "Exceptions/Exceptions.h"
#include "LevelObject/LevelObject.h"

static const std::string kLevelsFullPath = "LevelsFullPath";
static const std::string kSeed = "Seed";
static const std::string kLimit = "Limit";
static const std::string kCFG= "CFG";

struct  SOneLevelConfig
{
  void Load(
      const std::string _fileName
    )
  {
    Json::Value root;
    Json::Reader reader;
    std::ifstream fileStream(_fileName, std::ifstream::binary);

    std::string inputJsonString;
    inputJsonString.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

    if (inputJsonString[0] == char(0xEF) && inputJsonString[1] == char(0xBB) && inputJsonString[2] == char(0xBF))
      inputJsonString = inputJsonString.substr(3);

    if (!reader.parse(inputJsonString, root, false) || root.isNull())
      throw MJToolException("Can't parse config " + _fileName);

    m_LevelFullPath = root.get(kLevelsFullPath, "").asString();
    if (kLevelsFullPath.empty())
      throw MJToolException(kLevelsFullPath + " not specified.");

    int iSeed = -1;
    iSeed = root.get(kSeed, -1).asInt();
    if (iSeed < 0)
      throw MJToolException(kSeed + " not specified.");
    m_seedNumber = iSeed;

    int iLimit= -1;
    iLimit = root.get(kLimit, -1).asInt();
    if (iLimit < 0)
      throw MJToolException(kLimit + " not specified.");
    m_limit = iLimit;

    Json::Value cfg = root[kCFG];
    if (cfg.isNull() || !cfg.isObject() || !cfg.size())
      throw MJToolException(kCFG + " not specified.");

    m_cfg.Load(cfg, "NO_NAME");
  }

  std::string m_LevelFullPath;
  size_t      m_seedNumber;
  size_t      m_limit;
  SCFG        m_cfg;
};

inline void ExitMassage()
{
  std::cout << "\nPress any key for exit...\n";
  std::getchar();
}

int main()
{
  SOneLevelConfig config;
  try
  {
#ifdef DEBUG
    config.Load(R"(d:\develop\MJLevelTool\latest_build\one_level_config.json)");
#else
    config.Load("one_level_config.json");
#endif
  }
  catch (std::exception & e)
  {
    std::cerr << "Can't load config. Reason: " << e.what() << '\n';
    ExitMassage();
    return 0;
  }
  std::cout << "Read config...Ok\n";

  MJLevelObject levelObject;
  levelObject.InitWithDictionary(config.m_LevelFullPath);
  levelObject.SetRepeatTypeTimes(1);
  levelObject.SetCFG(config.m_cfg);
  if (!levelObject.BuildWithSeed(config.m_seedNumber))
  {
    std::cout << "Can't build.\n";
    ExitMassage();
    return 0;
  }

  std::cout << "Level build...Ok\nPlease wait...";
  std::cout << "\rReady.             \nFails rate: " << levelObject.PlayRandomNTimes(config.m_limit) << '\n';
  ExitMassage();
  return 0;
}