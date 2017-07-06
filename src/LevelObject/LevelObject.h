#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "Utils/Constants.h"
#include "Chip.h"
#include "Randomizer/Randomizer.h"

class MJLevelObject
{
public:
  MJLevelObject();

  void initWithDictionary(
      const std::string & _levelFileName
    );

  void SetCFG(
      const SCFG & _cfg
    );

  void BuildWithSeed(
      int _seed
    ) {}

private:
  void Clear();

  void AddChipPairType(
      MJChipGroup _group,
      int         _count,
      bool        _useRandom
    );

private:
  MJRandomizer        m_randomiser;
  std::vector<MJChip> m_levelChips;
  std::set<MJChip *>  m_startChips;

  std::vector<SPoint2d>                            m_firstGoldenChipPositions;
  std::vector<SPoint2d>                            m_secondGoldenChipPositions;
  std::vector<std::pair<std::string, std::string>> m_chipsTypes;

  SCFG           m_currentCFG;
  EAlgorithmType m_Type;
};

#endif // LEVELOBJECT_H
