#include "LevelObject/Chip.h"
#include "LevelObject/LevelObject.h"
#include "Utils/Constants.h"
#include "Randomizer.h"
//#include "R8Utilities.h"
//#include "MJGameManager.h"
//#include "R8Utilities_String.h"
//#include "MJGameScene.h"
//#include "MJLevelObject.h"

//const int kTryIterationsCount = 1000;
//static int iLocalChipNumber = 0;

//MJRandomizer::MJRandomizer()
//{

//}

//bool MJRandomizer::ValidateGoldChips(
//  const std::vector<MJChip *> & _chips,
//  const cocos2d::CCPoint &      _firstGoldPosition,
//  const cocos2d::CCPoint &      _secondGoldPosition
//  )
//{
//  MJChip * first = nullptr;
//  MJChip * second = nullptr;
//  for (auto chip : _chips)
//  {
//    if ((chip->getCellPosition().equals(_firstGoldPosition)) &&
//      (chip->getZLayer() == 0))
//    {
//      first = chip;
//    }
//    else if ((chip->getCellPosition().equals(_secondGoldPosition)) &&
//      (chip->getZLayer() == 0))
//    {
//      second = chip;
//    }
//  }

//  CCAssert(first != nullptr, "invalid first gold position");
//  CCAssert(second != nullptr, "invalid second gold position");

//  if (abs(_firstGoldPosition.x - _secondGoldPosition.x) <= 1.0f)
//    return true;

//  std::set<MJChip *> left = getAllLeftNeighbors(first);
//  std::set<MJChip *> right = getAllRightNeighbors(first);

//  return  (left.find(second) == left.end()) && (right.find(second) == right.end());
//}

//bool MJRandomizer::SetupGoldChips(
//    const std::vector<MJChip *> &                            _initialChips,
//    const std::vector<cocos2d::CCPoint> &                    _firstGoldenChipPositions,
//    const std::vector<cocos2d::CCPoint> &                    _secondGoldenChipPositions,
//    const cocos2d::CCPoint &                                 _chipOffset,
//    CRandomWrapper &                                         _generator,
//    std::set<MJChip *> &                                     _goldenChips
//  )
//{
//  int firstIndex = _generator.GetRandomValue(0, _firstGoldenChipPositions.size() - 1);
//  int secondIndex = _generator.GetRandomValue(0, _secondGoldenChipPositions.size() - 1);

//  CCPoint firstGoldPos = _firstGoldenChipPositions.at(firstIndex);
//  CCPoint secondGoldPos = _secondGoldenChipPositions.at(secondIndex);

//  int currentChipVariationCount = 0;
//  int currentChipsCombinationCount = 0;

//  firstGoldPos = ccpAdd(firstGoldPos, ccp(_chipOffset.x, _chipOffset.y + .5f));
//  secondGoldPos = ccpAdd(secondGoldPos, ccp(_chipOffset.x, _chipOffset.y + .5f));
//  CCAssert(ValidateGoldChips(_initialChips, firstGoldPos, secondGoldPos), "not valid gold chip position");

//  for (auto chip : _initialChips)
//  {
//    if (
//      (chip->getCellPosition().equals(firstGoldPos) || chip->getCellPosition().equals(secondGoldPos)) &&
//      chip->getZLayer() == 0)
//    {
//      chip->setType(MJChipTypeGolden);
//      _goldenChips.insert(chip);
//    }
//  }

//  {
//    // validate chips positions
//    bool m_bEqualPos = false;
//    for (auto chip : _initialChips)
//    {
//      for (auto xChip : _initialChips)
//      {
//        if (xChip == chip)
//          continue;

//        if (chip->getCellPosition().equals(xChip->getCellPosition()) && (chip->getZLayer() == xChip->getZLayer()))
//        {
//          m_bEqualPos = true;
//          CCLOG("pos: (%f, %f, %i)", chip->getCellPosition().x, chip->getCellPosition().y, chip->getZLayer());
//        }
//      }
//    }

//    CCAssert(!m_bEqualPos, "invalid chip position ");
//    if (m_bEqualPos)
//      return false;
//  }

//  {
//    // validate chips values
//    for (auto chip : _initialChips)
//    {
//      if (chip->getTypeValue() != "g0")
//      {
//        CCAssert(false, "invalid chips");
//        return false;
//        break;
//      }
//    }
//  }

//  return true;
//}

//std::pair<std::string, std::string> GetListElementByIndexAndErase(
//    std::list<std::pair<std::string, std::string>> & _list,
//    const int                                        _index
//  )
//{
//  std::pair<std::string, std::string> resultPair;
//  auto it = _list.begin();
//  for (size_t j = 0; j < _index; ++j)
//    ++it;

//  resultPair = *it;
//  _list.erase(it);

//  return resultPair;
//}

//bool MJRandomizer::RandomizeChips(
//  const std::vector<MJChip *> &                            _initialChips,
//  const std::vector<std::pair<std::string, std::string>> & _standardChipsValues,
//  const std::vector<cocos2d::CCPoint> &                    _firstGoldenChipPositions,
//  const std::vector<cocos2d::CCPoint> &                    _secondGoldenChipPositions,
//  const cocos2d::CCPoint &                                 _chipOffset,
//  CRandomWrapper &                                         _generator,
//  std::vector<MJChip *> &                                  _outChips,
//  std::set<MJChip *> &                                     _goldenChips,
//  SToolSettings &                                   _params
//  )
//{
//  if (!SetupGoldChips(
//        _initialChips,
//        _firstGoldenChipPositions,
//        _secondGoldenChipPositions,
//        _chipOffset,
//        _generator,
//        _goldenChips
//      )
//    )
//  {
//    return false;
//  }

//  {
//    iLocalChipNumber = 0;
//    int currentStandardChipsValuesIndex = 0;
//    std::vector<MJChip *> randomizingChip(_initialChips);
//    std::list<std::pair<std::string, std::string>> typesCopy;
//    std::pair<std::string, std::string> selectedTypesPair;
//    int flowerAndSeasonTypesCount = 2;
//    int repeatTypeCount = -1;

//    while (randomizingChip.size() > 2)
//    {
//      std::vector<MJChip *> vacantChips = getVacantChips(randomizingChip);
//      if (vacantChips.size() < 2)
//        return false;

//      if (typesCopy.empty())
//        typesCopy.assign(_standardChipsValues.begin(), _standardChipsValues.end());

//      int firstIndex = _generator.GetRandomValue(0, vacantChips.size() - 1);
//      MJChip * firstChip = vacantChips.at(firstIndex);

//      vacantChips.erase(vacantChips.begin() + firstIndex);

//      int secondIndex = _generator.GetRandomValue(0, vacantChips.size() - 1);
//      MJChip * secondChip = vacantChips.at(secondIndex);

//      CCAssert(firstChip != secondChip, "Chips must be different!");

//      if (repeatTypeCount < 0)
//      {
//        repeatTypeCount = 0;
//        if (_params.m_UseRandomType)
//        {
//          int index = _generator.GetRandomValue(0, _standardChipsValues.size() - 1);
//          selectedTypesPair = _standardChipsValues.at(index);
//        }
//        else
//        {
//          int notRandomTypesCount = typesCopy.size() - flowerAndSeasonTypesCount;
//          if (notRandomTypesCount > 0)
//          {
//            int randIndex = _generator.GetRandomValue(0, notRandomTypesCount);
//            selectedTypesPair = GetListElementByIndexAndErase(typesCopy, randIndex);
//          }
//          else
//          {
//            assert(typesCopy.size() > 0 && flowerAndSeasonTypesCount > 0);
//            selectedTypesPair = typesCopy.front();
//            typesCopy.pop_front();
//          }
//        }
//      }

//      specifyTypePairChips(firstChip, secondChip, selectedTypesPair);
//      ++repeatTypeCount;
//      if (repeatTypeCount >= _params.m_RepeatTypeTimes)
//        repeatTypeCount = -1;

//      removeChipAndChipNeighbors(firstChip, randomizingChip);
//      removeChipAndChipNeighbors(secondChip, randomizingChip);
//    }

//    for (auto chip : _initialChips)
//    {
//      chip->setupNeighbors(_initialChips);
//    }
//  }

//  _outChips = _initialChips;
//  return true;
//}

//void MJRandomizer::forceChipsSetup(std::vector<MJChip *> & _chips, MJChip *pVacantChip, const std::pair<std::string, std::string> & _oneChipType)
//{
//    MJChip * firstChip = pVacantChip;
//    MJChip * secondChip = nullptr;

//    for (auto it : _chips)
//    {
//        if (it != firstChip && it->getType() != MJChipTypeGolden)
//        {
//            secondChip = it;
//            break;
//        }
//    }

//    specifyTypePairChips(firstChip, secondChip, _oneChipType );
//    removeChipAndChipNeighbors(firstChip, _chips);
//    removeChipAndChipNeighbors(secondChip, _chips);
//}


//void MJRandomizer::removeChipAndChipNeighbors( MJChip * _chip, std::vector<MJChip *> & _chips )
//{
//  auto it = find(_chips.begin(), _chips.end(), _chip);
//  assert(it != _chips.end() && "chip not found in level chips");
//  if (it != _chips.end())
//    _chips.erase(it);

//  auto & neighborsRight = _chip->getNeighborsRight();
//  auto & neighborsBottom = _chip->getNeighborsBottom();
//  auto & neighborsLeft = _chip->getNeighborsLeft();

//  if (neighborsBottom.size() > 0 )
//  {
//    for ( auto chip : neighborsBottom)
//      chip->getNeighborsTop().erase( _chip );
//  }

//  if (neighborsLeft.size() > 0 )
//  {
//    for ( auto chip : neighborsLeft)
//      chip->getNeighborsRight().erase( _chip );
//  }

//  if (neighborsRight.size() > 0 )
//  {
//    for ( auto chip : neighborsRight)
//      chip->getNeighborsLeft().erase( _chip );
//  }

//  _chip->killAllNeighbors();
//}

//void MJRandomizer::specifyTypePairChips( MJChip *firstChip, MJChip *secondChip, const std::pair<string, string> & _oneChipType )
//{
//  firstChip->setTypeValue( _oneChipType.first );
//  secondChip->setTypeValue( _oneChipType.second );

//  firstChip->setNumber( iLocalChipNumber );
//  iLocalChipNumber++;

//  secondChip->setNumber( iLocalChipNumber );
//  iLocalChipNumber++;

//  firstChip->setPairChip( secondChip );
//  secondChip->setPairChip( firstChip );
//}

//std::vector<MJChip *> MJRandomizer::getVacantChips(const std::vector<MJChip *> & _chips)
//{
//  std::vector<MJChip *> vacantChips;
//  for (auto chip : _chips)
//  {
//    if (
//      (chip->getNeighborsTop().size() > 0) ||
//      (chip->getNeighborsRight().size() > 0 && chip->getNeighborsLeft().size() > 0) ||
//      (chip->getType() == MJChipTypeGolden)
//      )
//    {
//      continue;
//    }
//    else
//    {
//      vacantChips.push_back(chip);
//    }
//  }
//  return vacantChips;
//}


//struct sort_pred
//{
//  bool operator()(const std::pair<int, MJChip *> & _left, const std::pair<int, MJChip *> & _right)
//  {
//    return _left.first > _right.first;
//  }
//};

//const std::vector<std::pair<int, MJChip *> > MJRandomizer::getIndexAllVacantChips(const std::vector<MJChip *> & _chips)
//{
//  std::vector <std::pair<int, MJChip *> > indexVector;
//  indexVector.reserve(_chips.size());
//  for (auto it : _chips)
//  {
//    int iChipsNeighborsCount = 0;
//    iChipsNeighborsCount += it->getNeighborsBottom().size();
//    iChipsNeighborsCount += it->getNeighborsLeft().size();
//    iChipsNeighborsCount += it->getNeighborsRight().size();

//    std::pair<int, MJChip *> currentIndexPair;
//    currentIndexPair.first = iChipsNeighborsCount;
//    currentIndexPair.second = it;
//    indexVector.push_back(currentIndexPair);
//  }
//  std::sort(indexVector.begin(), indexVector.end(), sort_pred());

//  return indexVector;
//}


//set<MJChip *> MJRandomizer::getAllRightNeighbors(MJChip* chip)
//{
//    set<MJChip *>chips;

//    for(auto aChip : chip->getNeighborsRight())
//    {
//        chips.insert(aChip);
//        for(auto rChip : getAllRightNeighbors(aChip))
//            chips.insert(rChip);
//    }
//    return chips;
//}

//set<MJChip *> MJRandomizer::getAllLeftNeighbors(MJChip* chip)
//{
//  std::set<MJChip *>chips;

//  for (auto aChip : chip->getNeighborsLeft())
//  {
//    chips.insert(aChip);
//    for (auto lChip : getAllLeftNeighbors(aChip))
//      chips.insert(lChip);
//  }

//  return chips;
//}

//set<MJChip *> MJRandomizer::getAllTopNeighbors(MJChip* chip)
//{
//    set<MJChip *>chips;

//    for(auto aChip : chip->getNeighborsTop())
//    {
//        chips.insert(aChip);
//        for(auto lChip : getAllTopNeighbors(aChip))
//            chips.insert(lChip);
//    }

//    return chips;
//}
