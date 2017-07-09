#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <algorithm>
#include <vector>
#include <set>
#include <random>

#include "Randomizer/RandomWrapper.h"
#include "Utils/Constants.h"
#include "Utils/Utils.h"
#include "Config/ToolConfig.h"

class MJChip;

class MJRandomizer
{
private:
  typedef std::pair<std::string, std::string> ChipTypePair;

public:
  MJRandomizer()
    : m_type(EAlgorithmType::regular_type)
  {}

  void SetType(
      EAlgorithmType _type
    );

  void SetRepeatTypeTimes(
      int _repeatTypeCount
    );

  void RandomizeChipsWithSeed(
      std::vector<MJChip> &             _initialChips,
      const std::vector<ChipTypePair> & _typesList,
      const std::vector<SPoint2d> &     _firstGoldenChipPositions,
      const std::vector<SPoint2d> &     _secondGoldenChipPositions,
      int                               _seed
    );

  void SetupGoldChips(
      std::vector<MJChip> &         _initialChips,
      const std::vector<SPoint2d> & _firstGoldenChipPositions,
      const std::vector<SPoint2d> & _secondGoldenChipPositions
    );

private:
  void ValidateGoldChips(
      std::vector<MJChip> & _initialChips,
      const SPoint2d &      _firstGoldPosition,
      const SPoint2d &      _secondGoldPosition
    );

  void ValidateChips(
      const std::vector<MJChip> & _initialChips
    );

  void SpecifyType(
      std::vector<MJChip> &             _initialChips,
      const std::vector<ChipTypePair> & _typesList
    );

  std::vector<MJChip *> FindVacantChips(
      const std::vector<MJChip *> & _chips
    );

  void RemoveChipAndChipNeighbors(
      MJChip *                _chip,
      std::vector<MJChip *> & _chips
    );

private:
  int            m_repeatTypeTimes;
  EAlgorithmType m_type;
  CRandomWrapper m_randomWrapper;
};
#endif // RANDOMIZER_H
