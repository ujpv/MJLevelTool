#ifndef TOOLCONFIG_H
#define TOOLCONFIG_H

#include "json/json.h"

struct SLevelParams
{
  SLevelParams();
  double m_DifficultyMin;
  double m_DifficultyMax;
  size_t m_Limit;
};

struct SCFG
{
  void Load(
      const Json::Value & _cfgElement
    );

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

  const SCFG & GetCGF(
      const std::string & _name
    );

private:
    CToolConfig();

private:
    std::map<std::string, SCFG> m_CFGs;
    std::vector<std::string>    m_useCFG;

    std::map<std::string, SLevelParams> m_levelsParams;
    std::vector<std::string>            m_levelsForCheck;
};

#endif // TOOLCONFIG_H
