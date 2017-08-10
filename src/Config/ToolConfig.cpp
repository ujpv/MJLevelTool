#include <fstream>
#include <climits>

#include "ToolConfig.h"
#include "Exceptions/Exceptions.h"

static const std::string kCFG = "CFG";
static const std::string kUseCFG = "UseCFG";
static const std::string kLevels = "Levels";
static const std::string kLevelsForCheck = "LevelsForCheck";
static const std::string kMin = "min";
static const std::string kMax = "max";
static const std::string kStart = "start";
static const std::string kLimit = "limit";
static const std::string kCount = "count";

static const std::string kPaths = "Paths";
static const std::string kLevelsPath = "LevelsPath";
static const std::string kOutPath = "OutPath";

static const std::string kReportFileName = "ReportFileName";
static const std::string kEstimateComplexityLimit = "EstimateComplexityLimit";
static const std::string kOnlyEstimateComplexity = "OnlyEstimateComplexity";
static const std::string kEstimateComplexitySeedCount = "EstimateComplexitySeedCount";
static const std::string kEstimateComplexityRanges = "EstimateComplexityRanges";

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
static const unsigned defaultLimit = 1000;

CToolConfig &CToolConfig::Instance()
{
  static CToolConfig instance;
  return instance;
}

CToolConfig::CToolConfig():
  m_currentPath(0),
  m_isOnlyEstimateComplexity(false),
  m_estimateComplexityLimit(0),
  m_estimateComplexitySeedCount(0)
{}

void CToolConfig::ParsePathSection(
    const Json::Value & _config
  )
{
  if (_config.isNull() || !_config.isArray() || !_config.size())
    throw MJToolException("There are no correct Paths section");

  for (size_t i = 0; i < _config.size(); ++i)
  {
    Json::Value paths = _config.get(i, Json::nullValue);
    if (paths.isNull() || !paths.isObject() || !paths.size())
      throw MJToolException("Error in item #" + std::to_string(i) + " in Paths section:\n" + _config.toStyledString());

    std::string levelsPath = paths.get(kLevelsPath, "").asString();
    if (levelsPath.empty())
      throw MJToolException("There are no correct LevelsPath number #" + std::to_string(i) + " in:\n" + _config.toStyledString());

    std::string outPath = paths.get(kOutPath, "").asString();
    if (outPath.empty())
      throw MJToolException("There are no correct OutPath number #" + std::to_string(i) + " in:\n" + _config.toStyledString());

    m_paths.push_back({levelsPath, outPath});
  }

  m_currentPath = 0;
}

const std::vector<std::pair<double, double> > & CToolConfig::GetComplexityRanges() const
{
  return m_ranges;
}

const SLevelParams & CToolConfig::GetLevelParams(
    const std::string & _levelName
  ) const
{
  std::map<std::string, SLevelParams>::const_iterator it = m_levelsParams.find(_levelName);
  if (it == m_levelsParams.end())
    throw MJToolException("GetLevelParams. Invalid level name: " + _levelName);
  return it->second;
}

size_t CToolConfig::GetEstimateComplexitySeedCount() const
{
  return m_estimateComplexitySeedCount;
}

size_t CToolConfig::GetEstimateComplexityLimit() const
{
  return m_estimateComplexityLimit;
}

bool CToolConfig::IsOnlyEstimateComplexity() const
{
  return m_isOnlyEstimateComplexity;
}

const std::string & CToolConfig::GetReportFileName() const
{
  return m_reportFileName;
}

void CToolConfig::SetCurrentPathIndex(
    size_t _index
  )
{
  m_currentPath = _index;
}

size_t CToolConfig::GetPathsCount() const
{
  return m_paths.size();
}

const std::string & CToolConfig::GetLevelsPath() const
{
  return m_paths[m_currentPath].first;
}

const std::string & CToolConfig::GetOutPath() const
{
  return m_paths[m_currentPath].second;
}

void CToolConfig::Init(
    std::string _fileName
  )
{
  // Root
  Json::Value root;
  Json::Reader reader;
  std::ifstream fileStream(_fileName, std::ifstream::binary);

  std::string inputJsonString;
  inputJsonString.assign(std::istreambuf_iterator<char>(fileStream), std::istreambuf_iterator<char>());

  if (inputJsonString[0] == char(0xEF) && inputJsonString[1] == char(0xBB) && inputJsonString[2] == char(0xBF))
    inputJsonString = inputJsonString.substr(3);

  if (!reader.parse(inputJsonString, root, false) || root.isNull())
    throw MJToolException("Can't parse config " + _fileName + " Message: " + reader.getFormattedErrorMessages());

  // Common
  m_isOnlyEstimateComplexity = root.get(kOnlyEstimateComplexity, false).asBool();
  if (m_isOnlyEstimateComplexity)
  {
    m_reportFileName = root.get(kReportFileName, "").asString();
    if (m_reportFileName.empty())
      throw MJToolException(kReportFileName + " not specified.");

    m_estimateComplexityLimit = root.get(kEstimateComplexityLimit, INT_MAX).asUInt();
    if (m_estimateComplexityLimit == INT_MAX)
      throw MJToolException(kEstimateComplexityLimit + " not specified.");

    m_estimateComplexitySeedCount = root.get(kEstimateComplexitySeedCount, INT_MAX).asUInt();
    if (m_estimateComplexitySeedCount == INT_MAX)
      throw MJToolException(kEstimateComplexitySeedCount + " not specified.");

    Json::Value ranges = root[kEstimateComplexityRanges];
    if (ranges.isNull() || !ranges.isArray() || !ranges.size())
      throw MJToolException("There are no correct " + kEstimateComplexityRanges + " section");

    for (Json::Value::const_iterator it = ranges.begin(); it != ranges.end(); ++it)
    {
      double min = it->get(kMin, -1.0).asDouble();
      if (min < 0)
        throw MJToolException("There are no correct min in " + kEstimateComplexityRanges + "section");
      double max = it->get(kMax, -1.0).asDouble();
      if (max < 0)
        throw MJToolException("There are no correct min in " + kEstimateComplexityRanges + "section");
      m_ranges.push_back({min, max});
    }
  }

  // CFG
  Json::Value CFGs = root[kCFG];
  if (CFGs.isNull() || !CFGs.isObject() || !CFGs.size())
    throw MJToolException("There are no correct " + kCFG + " section");

  for (Json::Value::const_iterator it = CFGs.begin(); it != CFGs.end(); ++it)
  {
    SCFG cfg;
    std::string blockName = it.name();
    if (blockName.empty())
      throw MJToolException("Can't parse block:\n" + it->toStyledString());
    cfg.Load(*it, blockName);
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

  // Paths
  ParsePathSection(root[kPaths]);

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

    cfg.m_DifficultyMin = it->get(kMin, defaultDifficultyMin).asDouble();
    cfg.m_DifficultyMax = it->get(kMax, defaultDifficultyMax).asDouble();

    int value = it->get(kStart, -1).asInt();
    if (value == -1)
      throw MJToolException(blockName + ": not specifaied " + kStart);
    cfg.m_startSeed = static_cast<size_t>(value);

    cfg.m_Limit = it->get(kLimit, defaultLimit).asUInt();
    cfg.m_seedNeed = it->get(kCount, -1).asUInt();

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

      if (!m_levelsParams.count(name) && !m_isOnlyEstimateComplexity)
        throw MJToolException("Level params for " + name + " not specified:\n");


      m_levelsForCheck.push_back(it->asString());
    }
    catch (Json::LogicError & e)
    {
      throw MJToolException("Can't parse block: \n" + it->toStyledString() + "Reason:" + e.what());
    }
  }
}

const std::map<std::string, SCFG> & CToolConfig::GetCGFs() const
{
  return m_CFGs;
}

const std::vector<std::string> & CToolConfig::GetNamesOfCGF() const
{
  return m_useCFG;
}

const SLevelParams &CToolConfig::GetLevelParams(
    const std::string & _name
  )
{
  return m_levelsParams[_name];
}

const std::vector<std::string> & CToolConfig::GetLevelsForCheck() const
{
  return m_levelsForCheck;
}

void SCFG::Load(
    const Json::Value & _cfgElement,
    const std::string & _id
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
  m_id = _id;

  if (m_iFirstGroupDigitChipsNumber == -1 ||
      m_iSecondGroupDigitChipsNumber == -1 ||
      m_iThirdGroupDigitChipsNumber == -1 ||
      m_iMysticGroupChipsNumber == -1 ||
      m_iDragonChipGroupNumber == -1 ||
      m_iFlowerChipGroupNumber == -1 ||
      m_iSeasonChipGroupNumber == -1 ||
      m_iWindChipGroupNumber == -1)
  {
    throw MJToolException("Config error: Not specified CFG group:\n" + _cfgElement.toStyledString());
  }
}

SLevelParams::SLevelParams(): 
  m_DifficultyMin(defaultDifficultyMin), 
  m_DifficultyMax(defaultDifficultyMax), 
  m_startSeed(0), 
  m_seedNeed(1), 
  m_Limit(defaultLimit)
{}
