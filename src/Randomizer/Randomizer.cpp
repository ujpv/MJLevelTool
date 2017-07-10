#include <list>
#include <set>

#include "Exceptions/Exceptions.h"
#include "LevelObject/Chip.h"
#include "Utils/Constants.h"
#include "Randomizer.h"
#include "Randomizer/RandomWrapper.h"

void MJRandomizer::SetType(
    EAlgorithmType _type
  )
{
  m_type = _type;
  if (_type == EAlgorithmType::random_type)
    m_randomWrapper.SetType(CRandomWrapper::eRandomType::stdrandom);
  else
    m_randomWrapper.SetType(CRandomWrapper::eRandomType::match3random);
}

void MJRandomizer::SetRepeatTypeTimes(int _repeatTypeCount)
{
  m_repeatTypeTimes = _repeatTypeCount;
}

void MJRandomizer::RandomizeChipsWithSeed(
    std::vector<MJChip> &             _initialChips,
    const std::vector<ChipTypePair> & _typesList,
    const std::vector<SPoint2d> &     _firstGoldenChipPositions,
    const std::vector<SPoint2d> &     _secondGoldenChipPositions,
    int                               _seed)
{
  m_randomWrapper.SetSeed(_seed);
  SetupGoldChips(
        _initialChips,
        _firstGoldenChipPositions,
        _secondGoldenChipPositions
      );

  ValidateChips(_initialChips);

  SpecifyType(_initialChips, _typesList);
}

void MJRandomizer::SetupGoldChips(
    std::vector<MJChip> &         _initialChips,
    const std::vector<SPoint2d> & _firstGoldenChipPositions,
    const std::vector<SPoint2d> & _secondGoldenChipPositions
  )
{
  std::int64_t firstIndex = m_randomWrapper.GetRandomValue(0, static_cast<int>(_firstGoldenChipPositions.size()) - 1);
  std::int64_t secondIndex = m_randomWrapper.GetRandomValue(0, static_cast<int>(_secondGoldenChipPositions.size()) - 1);

  SPoint2d firstGoldPos = _firstGoldenChipPositions.at(static_cast<size_t>(firstIndex));
  SPoint2d secondGoldPos = _secondGoldenChipPositions.at(static_cast<size_t>(secondIndex));
  ValidateGoldChips(_initialChips, firstGoldPos, secondGoldPos);

  for (MJChip & chip: _initialChips)
  {
    if ((chip.GetPosition().equals(firstGoldPos) ||
         chip.GetPosition().equals(secondGoldPos)) &&
        chip.GetZOrder() == 0)
    {
      chip.SetType(MJChipTypeGolden);
    }
  }
}

void MJRandomizer::ValidateGoldChips(
    std::vector<MJChip> & _initialChips,
    const SPoint2d &      _firstGoldPosition,
    const SPoint2d &      _secondGoldPosition
  )
{
  MJChip * pFirst = nullptr;
  MJChip * pSecond = nullptr;

  for (size_t i = 0; i < _initialChips.size(); ++i)
  {
    if (_initialChips[i].GetPosition().equals(_firstGoldPosition) &&
        _initialChips[i].GetZOrder() == 0)
    {
      pFirst = &_initialChips[i];
    }
    else
    if (_initialChips[i].GetPosition().equals(_secondGoldPosition) &&
        _initialChips[i].GetZOrder() == 0)
    {
      pSecond = &_initialChips[i];
    }
  }

  if (!pFirst || !pSecond)
    throw MJToolException(!pFirst ? "invalid first gold position" : "invalid second gold position");

  if (std::abs(_firstGoldPosition.x - _secondGoldPosition.x) <= 1.0f)
    return;

  const std::set<MJChip *> & left = pFirst->GetNeighbors(Left);
  const std::set<MJChip *> & right = pFirst->GetNeighbors(Right);

  if ((left.find(pFirst)   != left.end()) ||
      (right.find(pSecond) != right.end()))
  {
    throw MJToolException("SetupGoldChips not valid gold chip position");
  }
}

void MJRandomizer::ValidateChips(
    const std::vector<MJChip> & _initialChips
  )
{
  std::vector<std::pair<SPoint2d, int>> invalidChip;
  for (std::vector<MJChip>::const_iterator it1 = _initialChips.begin(); it1 != _initialChips.end(); ++it1)
  {
    for (std::vector<MJChip>::const_iterator it2 = _initialChips.begin(); it2 != _initialChips.end(); ++it2)
    {
      const MJChip * chip1 = &(*it1);
      const MJChip * chip2 = &(*it2);

      if (chip1 == chip2)
        continue;

      if (chip1->GetPosition().equals(chip2->GetPosition()) && chip1->GetZOrder() == chip2->GetZOrder())
        invalidChip.push_back({chip1->GetPosition(), chip1->GetZOrder()});
    }
  }

  if (!invalidChip.empty())
  {
    std::stringstream ss;
    ss << "Invalid chip position:\n";
    for (const std::pair<SPoint2d, int> & point: invalidChip)
      ss << "Point:" << point.first << ", Layer: " << point.second << '\n';

    throw MJToolException(ss.str());
  }

  for (const MJChip & chip: _initialChips)
  {
    if (chip.GetTypeValue() != INITAL_CHIP_TYPE_VALUE)
      throw MJToolException("Invalid chip type: " + chip.GetTypeValue());
  }
}

void MJRandomizer::SpecifyType(
    std::vector<MJChip> &             _initialChips,
    const std::vector<ChipTypePair> & _typesList
  )
{
  std::vector<MJChip *> randomizingChip;
  for (MJChip & chip : _initialChips)
    randomizingChip.push_back(&chip);

  std::list<ChipTypePair> typesCopy(_typesList.begin(), _typesList.end());
  //std::pair<std::string, std::string> selectedTypesPair;

  int flowerAndSeasonTypesCount = 2;
  int repeatTypeCount = -1;
  while (randomizingChip.size() > 2)
  {
    std::vector<MJChip *> vacantChips = FindVacantChips(randomizingChip);

    if (vacantChips.size() < 2)
      throw MJToolException("vacantChips.size() < 2");

    if (typesCopy.empty())
    {
      typesCopy.assign(_typesList.begin(), _typesList.end());
    }

    int64_t firstIndex = m_randomWrapper.GetRandomValue(0, static_cast<int>(vacantChips.size()) - 1);
    MJChip * firstChip = vacantChips.at(static_cast<size_t>(firstIndex));
    vacantChips.erase(vacantChips.begin() + static_cast<int>(firstIndex));

    int64_t secondIndex = m_randomWrapper.GetRandomValue(0, static_cast<int>(vacantChips.size()) - 1);
    MJChip * secondChip = vacantChips.at(static_cast<size_t>(secondIndex));
    vacantChips.erase(vacantChips.begin() + static_cast<int>(secondIndex));

    if (firstChip == secondChip)
      throw ("SpecifyType. Chips must be different.");


    ChipTypePair selectedTypesPair;
    if (repeatTypeCount < 0)
    {
      repeatTypeCount = 0;

      switch (m_type) {
        case EAlgorithmType::random_type:
        {
          int64_t index = m_randomWrapper.GetRandomValue(0, static_cast<int>(_typesList.size()) - 1);
          selectedTypesPair = _typesList.at(static_cast<size_t>(index));
          throw MJToolException("Unxepected algorithm. EAlgorithmType::random_type");
          break;
        }
        case EAlgorithmType::regular_type:
        {
          int notRandomTypesCount = static_cast<int>(typesCopy.size()) - flowerAndSeasonTypesCount;
          if (notRandomTypesCount > 0)
          {
            int64_t randIndex = m_randomWrapper.GetRandomValue(0, notRandomTypesCount);
            std::list<ChipTypePair>::iterator it = typesCopy.begin();
            while (randIndex--)
              ++it;
            selectedTypesPair = *it;
            typesCopy.erase(it);
          }
          else
          {
            if (typesCopy.empty() || flowerAndSeasonTypesCount <= 0)
              throw MJToolException("Illegal state. assert(typesCopy.size() > 0 && flowerAndSeasonTypesCount > 0)");

            selectedTypesPair = typesCopy.front();
            typesCopy.pop_front();
          }
          break;
        }
        default:
          throw MJToolException("Not valid algorithm type");
      }
    }

    firstChip->SetTypeValue(selectedTypesPair.first);
    secondChip->SetTypeValue(selectedTypesPair.second);

    ++repeatTypeCount;
    if (repeatTypeCount >= m_repeatTypeTimes)
      repeatTypeCount = -1;

    RemoveChipAndChipNeighbors(firstChip, randomizingChip);
    RemoveChipAndChipNeighbors(secondChip, randomizingChip);
  }

  for (MJChip & chip: _initialChips)
    chip.SetupNeighbors(_initialChips);
}

std::vector<MJChip *> MJRandomizer::FindVacantChips(
    const std::vector<MJChip *> & _chips
  )
{
  std::vector<MJChip *> vacantChips;
  for (MJChip * pChip: _chips)
    if (!pChip->IsBlockedByNeighbors() && pChip->GetType() != MJChipTypeGolden)
      vacantChips.push_back(pChip);

  return vacantChips;
}

void MJRandomizer::RemoveChipAndChipNeighbors(
    MJChip * _chip,
    std::vector<MJChip *> & _chips
  )
{
  std::vector<MJChip *>::iterator it = std::find(_chips.begin(), _chips.end(), _chip);
  if (it != _chips.end())
    _chips.erase(it);

  _chip->RemoveNeighbors();
}
