#ifndef TOOLCONFIG_H
#define TOOLCONFIG_H

#include "json/json.h"

struct SLevelParams
{
  SLevelParams();

  double m_DifficultyMin;
  double m_DifficultyMax;
  size_t m_startSeed;
  size_t m_seedNeed;
  size_t m_Limit;
};

struct SCFG
{
  void Load(
      const Json::Value & _cfgElement, 
      const std::string & _id
    );

  std::string m_id;

  int m_iFirstGroupDigitChipsNumber;
  int m_iSecondGroupDigitChipsNumber;
  int m_iThirdGroupDigitChipsNumber;
  int m_iMysticGroupChipsNumber;
  int m_iDragonChipGroupNumber;
  int m_iFlowerChipGroupNumber;
  int m_iSeasonChipGroupNumber;
  int m_iWindChipGroupNumber;
};

class CToolConfig
{
public:
  static CToolConfig & Instance();
  void Init(
      std::string _fileName
    );

  const std::map<std::string, SCFG> & GetCGFs() const;

  const std::vector<std::string> & GetNamesOfCGF() const;

  const SLevelParams & GetLevelParams(
      const std::string & _name
    );

  const std::vector<std::string> & GetLevelsForCheck() const;

  const std::string & GetLevelsPath() const;

  const std::string & GetOutPath() const;

  void SetCurrentPathIndex(
      size_t _index
    );

  size_t GetPathsCount() const;

  const std::string & GetReportFileName() const;

  bool IsOnlyEstimateComplexity() const;

  size_t GetEstimateComplexityLimit() const;

  size_t GetEstimateComplexitySeedCount() const;

  const std::vector<std::pair<double, double> > & GetComplexityRanges() const;

  const SLevelParams & GetLevelParams(
      const std::string & _levelName
    ) const;

private:
  CToolConfig();
  void ParsePathSection(
      const Json::Value & _config
    );

private:
  std::map<std::string, SCFG> m_CFGs;
  std::vector<std::string>    m_useCFG;

  std::map<std::string, SLevelParams> m_levelsParams;
  std::vector<std::string>            m_levelsForCheck;

  std::vector<std::pair<std::string, std::string>> m_paths; // (levelPaths, outPath)
  size_t                                           m_currentPath;

  std::string                            m_reportFileName;
  std::vector<std::pair<double, double>> m_ranges;
  bool                                   m_isOnlyEstimateComplexity;
  size_t                                 m_estimateComplexityLimit;
  size_t                                 m_estimateComplexitySeedCount;
};

#endif // TOOLCONFIG_H
