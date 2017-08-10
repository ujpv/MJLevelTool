#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "Utils/Constants.h"
#include "Chip.h"
#include "Randomizer/Randomizer.h"

class MJLevelObject
{
public:
  MJLevelObject();

  void InitWithDictionary(
      const std::string & _levelFileName
    );

  void SetCFG(
      const SCFG & _cfg
    );

  void SetRepeatTypeTimes(
      int _count
    );

  bool BuildWithSeed(
      int _seed
    );

  void SaveCache();

  void RestoreFromCache();

  const std::vector<MJChip> & GetChips() const;

  size_t GetLastSeed() const;

  bool PlayRandom();

  double PlayRandomNTimes(
      size_t _count
    );

private:
  void FindLastSeed(
      const Plist::dictionary_type & _extraParamsDict
    );

  void Clear();

  void AddChipPairType(
      MJChipGroup _group,
      int         _count,
      bool        _useRandom
    );

  void TryToAddActiveChips(
      MJChip * _chip
    );

  void RemoveFromActiveChipsAndUpdate(
      MJChip * _chip
    );

  std::pair<MJChip *, MJChip *> GetActivePair();

private:
  MJRandomizer        m_randomiser;
  std::vector<MJChip> m_levelChips;

  std::map<std::string, std::set<MJChip *>>                        m_activeChips;
  std::map<std::string, std::set<MJChip *>>                        m_activeChipsCache;
  std::vector<std::map<std::string, std::set<MJChip *>>::iterator> m_activeTypes;

  std::vector<SPoint2d>                            m_firstGoldenChipPositions;
  std::vector<SPoint2d>                            m_secondGoldenChipPositions;
  std::vector<std::pair<std::string, std::string>> m_chipsTypes;

  EAlgorithmType m_type;
  size_t         m_lastSeed;
};

#endif // LEVELOBJECT_H
