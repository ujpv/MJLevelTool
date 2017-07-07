#include <fstream>

#include "ToolConfig.h"
#include "Exceptions/Exceptions.h"

static const std::string kCFG = "CFG";
static const std::string kUseCFG = "UseCFG";
static const std::string kLevels = "Levels";
static const std::string kLevelsForCheck = "LevelsForCheck";
static const std::string kMin = "min";
static const std::string kMax = "max";

static const char * kDigitChipFirstGroupNumber = "DigitChipFirstGroup";
static const char * kDigitChipSecondGroupNumber = "DigitChipSecondGroup";
static const char * kDigitChipThirdGroupNumber = "DigitChipThirdGroup";
static const char * kDragonChipGroupNumber = "DragonChipGroup";
static const char * kFlowerChipGroupNumber = "FlowerChipGroup";
static const char * kMysticChipGroupNumber = "MysticChipGroup";
static const char * kSeasonChipGroupNumber = "SeasonChipGroup";
static const char * kWindChipGroupNumber = "WindChipGroup";

static const double defaultDifficultyMin = 0.0;
static const double defaultDifficultyMax = 1.0;
static const unsigned defaultLimit = 10000;

CToolConfig &CToolConfig::Instance()
{
  static CToolConfig instance;
  return instance;
}

CToolConfig::CToolConfig() {}

void CToolConfig::Init(
    std::string _fileName
    )
{
  // Root
  Json::Value root;
  Json::Reader reader;
  std::ifstream fileStream(_fileName, std::ifstream::binary);
  if (!reader.parse(fileStream, root, false) || root.isNull())
    throw MJToolException("Can't parse config " + _fileName);

  // CFG
  Json::Value CFGs = root[kCFG];
  if (CFGs.isNull() || !CFGs.isObject() || !CFGs.size())
    throw MJToolException("There are no correct " + kCFG + " section");

  for (Json::Value::const_iterator it = CFGs.begin(); it != CFGs.end(); ++it)
  {
    SCFG cfg;
    std::string blockName = it.name();
    if (blockName.empty())
      throw MJToolException("Can't parse block: \n" + it->toStyledString());
    cfg.Load(*it);
    m_CFGs[blockName] = cfg;
  }

  // UseCFG
  Json::Value useCFGs = root[kUseCFG];
  if (useCFGs.isNull() || !useCFGs.isArray() || !useCFGs.size())
    throw MJToolException("UseCFG. There are no correct " + kUseCFG + " section");

  for (Json::Value::const_iterator it = useCFGs.begin(); it != useCFGs.end(); ++it)
  {
    try
    {
      std::string name = it->asString();
      if (!m_CFGs.count(name))
        throw MJToolException("There are no key " + name + " in " + kCFG + " section");

      m_useCFG.push_back(it->asString());
    }
    catch (Json::LogicError & e)
    {
      throw MJToolException("Can't parse block: \n" + it->toStyledString() + "\nReason: " + e.what());
    }
  }

  // Levels
  Json::Value levels = root[kLevels];
  if (levels.isNull() || !levels.isObject() || !levels.size())
    throw MJToolException("There are no correct " + kLevels + " section:\n" + levels.toStyledString());

  for (Json::Value::const_iterator it = levels.begin(); it != levels.end(); ++it)
  {
    SLevelParams cfg;
    std::string blockName = it.name();
    if (blockName.empty())
      throw MJToolException("Can't parse block: \n" + it->toStyledString());

    cfg.m_DifficultyMin = it->get(kMin, 0.0).asDouble();
    cfg.m_DifficultyMax = it->get(kMax, 1.0).asDouble();
    m_levelsParams[blockName] = cfg;
  }

  // Levels for check
  Json::Value levelsForCheck = root[kLevelsForCheck];
  if (levelsForCheck.isNull() || !levelsForCheck.isArray() || !levelsForCheck.size())
    throw MJToolException("LevelsForCheck. There are no correct " + kLevelsForCheck + " section:\n" + levelsForCheck.toStyledString());

  for (Json::Value::const_iterator it = levelsForCheck.begin(); it != levelsForCheck.end(); ++it)
  {
    try
    {
      std::string name = it->asString();
      if (name.empty())
        throw MJToolException("LevelsForCheck. Name should not be empty.\n");

      if (!m_levelsParams.count(name))
      {
        // default values
        m_levelsParams[name] = SLevelParams();
      }

      m_levelsForCheck.push_back(it->asString());
    }
    catch (Json::LogicError & e)
    {
      throw MJToolException("Can't parse block: \n" + it->toStyledString() + "Reason:" + e.what());
    }
  }
}

const SCFG &CToolConfig::GetCGF(
    const std::string & _name
  )
{
  std::map<std::string, SCFG>::const_iterator it = m_CFGs.find(_name);
  if (it == m_CFGs.end())
    throw MJToolException("Config error: Can't find CFG for name: " + _name);
  return it->second;
}

void SCFG::Load(
    const Json::Value & _cfgElement
    )
{
  m_iFirstGroupDigitChipsNumber =  _cfgElement.get(kDigitChipFirstGroupNumber,   -1).asInt();
  m_iSecondGroupDigitChipsNumber = _cfgElement.get(kDigitChipSecondGroupNumber,  -1).asInt();
  m_iThirdGroupDigitChipsNumber =  _cfgElement.get(kDigitChipThirdGroupNumber,   -1).asInt();
  m_iMysticGroupChipsNumber =      _cfgElement.get(kMysticChipGroupNumber,       -1).asInt();
  m_iDragonChipGroupNumber =       _cfgElement.get(kDragonChipGroupNumber,       -1).asInt();
  m_iFlowerChipGroupNumber =       _cfgElement.get(kFlowerChipGroupNumber,       -1).asInt();
  m_iSeasonChipGroupNumber =       _cfgElement.get(kSeasonChipGroupNumber,       -1).asInt();
  m_iWindChipGroupNumber =         _cfgElement.get(kWindChipGroupNumber,         -1).asInt();

  if (m_iFirstGroupDigitChipsNumber == -1 ||
      m_iSecondGroupDigitChipsNumber == -1 ||
      m_iThirdGroupDigitChipsNumber == -1 ||
      m_iMysticGroupChipsNumber == -1 ||
      m_iDragonChipGroupNumber == -1 ||
      m_iFlowerChipGroupNumber == -1 ||
      m_iSeasonChipGroupNumber == -1 ||
      m_iWindChipGroupNumber == -1)
  {
    throw MJToolException("Config error: Not specified CFG group.");
  }
}

SLevelParams::SLevelParams()
  : m_DifficultyMin(defaultDifficultyMin)
  , m_DifficultyMax(defaultDifficultyMax)
  , m_Limit(defaultLimit)
{}
