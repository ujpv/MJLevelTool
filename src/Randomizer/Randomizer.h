#ifndef RANDOMIZER_H
#define RANDOMIZER_H

#include <algorithm>
#include <vector>
#include <set>
#include <random>

#include "Utils/Utils.h"
#include "Config/ToolConfig.h"

//class MJChip;
//class CRandomWrapper;

class MJRandomizer
{
//  void forceChipsSetup(
//      std::vector<MJChip *> &                     _chipsSet,
//      MJChip *                                    _pVacantChip,
//      const std::pair<std::string, std::string> & _oneChipType
//    );

//public:
//  MJRandomizer();
//  ~MJRandomizer() {}

//  bool ValidateGoldChips(
//      const std::vector<MJChip *> & _chips,
//      const SPoint2d &              _firstGoldPosition,
//      const SPoint2d &              _secondGoldPosition
//    );

//  bool SetupGoldChips(
//      const std::vector<MJChip *> &                            _InitialChips,
//      const std::vector<SPoint2d> &                            _firstGoldenChipPositions,
//      const std::vector<SPoint2d> &                            _secondGoldenChipPositions,
//      const SPoint2d &                                         _chipOffset, // TODO remove it
//      CRandomWrapper &                                         _generator,
//      std::set<MJChip *> &                                     _goldenChips
//    );

//  bool RandomizeChips(
//      const std::vector<MJChip *> &                            _InitialChips,
//      const std::vector<std::pair<std::string, std::string>> & _standardChipsValues,
//      const std::vector<SPoint2d> &                            _firstGoldenChipPositions,
//      const std::vector<SPoint2d> &                            _secondGoldenChipPositions,
//      const SPoint2d &                                         _chipOffset, // TODO remove it
//      CRandomWrapper &                                         _generator,
//      std::vector<MJChip *> &                                  _outChips,
//      std::set<MJChip *> &                                     _goldenChips,
//      CToolConfig &                                            _config
//    );

//  std::vector<MJChip *> getVacantChips(
//      const std::vector<MJChip *> & _chipsSet
//    );

//  const std::vector<std::pair<int, MJChip *> > getIndexAllVacantChips(
//      const std::vector<MJChip *> & _chips
//    );

//  void specifyTypePairChips(
//      MJChip *firstChip,
//      MJChip *secondChip,
//      const std::pair<std::string, std::string> & _oneChipType
//    );

//  void removeChipAndChipNeighbors(
//      MJChip * _chip,
//      std::vector<MJChip *> & _chips
//    );

//  std::set<MJChip *> getAllRightNeighbors(
//      MJChip* chip
//    );
//  std::set<MJChip *> getAllLeftNeighbors(
//      MJChip* chip
//    );
//  std::set<MJChip *> getAllTopNeighbors(
//      MJChip* chip
//    );
};
#endif // RANDOMIZER_H
