#include "Exceptions/Exceptions.h"
#include "LevelObject.h"
#include "Utils/Constants.h"

static const std::string kChipLayers = "Layers";
static const std::string kFirstGoldenChipPosition = "FirstGoldenChipPosition";
static const std::string kSecondGoldenChipPosition = "SecondGoldenChipPosition";

static const std::string kMetadata    = "Metadata";
static const std::string kExtraParams = "ExtraParams";
static const std::string kLastSeed    = "LastSeed";
static const std::string kPresets     = "Presets";
static const std::string kSeed        = "Seed";

const std::vector<MJChip> & MJLevelObject::GetChips() const
{
  return m_levelChips;
}

size_t MJLevelObject::GetLastSeed() const
{
  return m_lastSeed;
}

bool MJLevelObject::PlayRandom()
{
  RestoreFromCache();

  size_t pairesCount = m_levelChips.size() / 2;
  for (size_t i = 0; i < pairesCount; ++i)
  {
    std::pair<MJChip *, MJChip *> chips = GetActivePair();
    if (!chips.first || !chips.second)
      return false;

    if (chips.first->GetType() == MJChipTypeGolden || chips.first->GetTypeValue()[0] == 'g')
    {
      if (chips.second->GetType() == MJChipTypeGolden || chips.second->GetTypeValue()[0] == 'g')
        return true;
      throw MJToolException("Illegal state. chips.second->GetType() != MJChipTypeGolden.");
    }

    chips.first->RemoveFromNeighbors();
    RemoveFromActiveChipsAndUpdate(chips.first);
    chips.first->ClearNeighborsContainers();

    chips.second->RemoveFromNeighbors();
    RemoveFromActiveChipsAndUpdate(chips.second);
    chips.second->ClearNeighborsContainers();
  }
  throw MJToolException("Illegal state. Not collected gold chips in m_levelChips.size() / 2 moves.");
}

double MJLevelObject::PlayRandomNTimes(size_t _count)
{
  size_t succesesCount = 0;
  size_t failCount = 0;
  while (_count--) {
    if (PlayRandom())
      ++succesesCount;
    else
      ++failCount;
  }
  return static_cast<double>(failCount) / static_cast<double>(succesesCount + failCount);
}

void MJLevelObject::FindLastSeed(
    const Plist::dictionary_type & _extraParamsDict
  )
{
  int lastSeed = -1;
  PlistUtils::getIntForKey(_extraParamsDict, kLastSeed, lastSeed);
  if (lastSeed >= 0)
  {
    m_lastSeed = static_cast<size_t>(lastSeed);
    return;
  }

  const Plist::array_type * pPresets = PlistUtils::getArrayForKey(_extraParamsDict, kPresets);
  if (!pPresets || pPresets->empty())
    return;

  for (size_t i = 0; i < pPresets->size(); ++i)
  {
    const Plist::dictionary_type * preset = PlistUtils::getDictForIndex(*pPresets, int(i));
    if (!preset)
      throw MJToolException("Illegal state: preset == nullptr");

    std::string sSeed;
    if (PlistUtils::getStringForKey(*preset, kSeed, sSeed) && !sSeed.empty())
      m_lastSeed = std::max(m_lastSeed, static_cast<size_t>(std::stoi(sSeed) + 1)); ;
  }
}

MJLevelObject::MJLevelObject(): 
  m_type(EAlgorithmType::regular_type), 
  m_lastSeed(0)
{
  m_randomiser.SetType(m_type);
}

void MJLevelObject::InitWithDictionary(
    const std::string & _levelFileName
  )
{
  Plist::dictionary_type params;
  try
  {
    Plist::readPlist(_levelFileName.c_str(), params);
  }
  catch (Plist::Error & e)
  {
    throw MJToolException("Can't read " + _levelFileName + ". Reason: \n" + e.what());
  }

  const Plist::array_type * pLayersArray = PlistUtils::getArrayForKey(params, kChipLayers);
  if (!pLayersArray)
    throw MJToolException("There is no " + kChipLayers + " section");

  int layersCount = static_cast<int>(pLayersArray->size());
  if (!layersCount)
    throw MJToolException("Section " + kChipLayers + " is empty");

  Clear();

  for (int iLayer = layersCount - 1; iLayer >= 0; --iLayer)
  {
    const Plist::array_type * pChipsArray = PlistUtils::getArrayForIndex(*pLayersArray, iLayer);
    if (!pChipsArray || pChipsArray->empty())
      throw MJToolException("Chips layer #" + std::to_string(iLayer) + " empty");

    for (size_t iChip = 0; iChip < pChipsArray->size(); ++iChip)
    {
      m_levelChips.push_back(MJChip());
      MJChip & chip = m_levelChips.back();

      if (!chip.InitWithParameters(&pChipsArray->at(iChip), iLayer))
        throw MJToolException("Cat't read params for layer#" + std::to_string(iLayer) + " chip #" + std::to_string(iChip));

      if (chip.GetTypeValue().find(ChipUtils::GetGroupPrefix(MJGoldChipGroup)) == std::string::npos)
        chip.SetTypeValue("g0");
    }
  }

  for (MJChip & chip: m_levelChips)
    chip.SetupNeighbors(m_levelChips);

  const Plist::dictionary_type * pMetadata = PlistUtils::getDictForKey(params, kMetadata);
  if (!pMetadata)
    throw MJToolException("There is no " + kMetadata + " section");

  const Plist::array_type * pFirstGoldenChipPositions = PlistUtils::getArrayForKey(*pMetadata, kFirstGoldenChipPosition);
  const Plist::array_type * pSecondGoldenChipPositions = PlistUtils::getArrayForKey(*pMetadata, kSecondGoldenChipPosition);

  if (!pFirstGoldenChipPositions || pFirstGoldenChipPositions->empty() ||
      !pSecondGoldenChipPositions || pSecondGoldenChipPositions->empty())
  {
    throw MJToolException("Golden positions not specified");
  }

  m_firstGoldenChipPositions.clear();
  m_secondGoldenChipPositions.clear();
  for (size_t i = 0; i < pFirstGoldenChipPositions->size(); ++i)
  {
    std::string sPosition;
    if (!PlistUtils::getStringForIndex(*pFirstGoldenChipPositions, int(i), sPosition))
      throw MJToolException("Golden positions not specified");

    SPoint2d point;
    if (!ParsePoint2d(sPosition, point))
      throw MJToolException("Can't parse first gold ship #" + std::to_string(i));

    m_firstGoldenChipPositions.push_back(point);
  }

  for (size_t i = 0; i < pSecondGoldenChipPositions->size(); ++i)
  {
    std::string sPosition;
    if (!PlistUtils::getStringForIndex(*pSecondGoldenChipPositions, int(i), sPosition))
      throw MJToolException("Golden positions not specified");

    SPoint2d point;
    if (!ParsePoint2d(sPosition, point))
      throw MJToolException("Can't parse second gold ship #" + std::to_string(i));

    m_secondGoldenChipPositions.push_back(point);
  }

  const Plist::dictionary_type * pExtraParams = PlistUtils::getDictForKey(params, kExtraParams);
  if (pExtraParams)
    FindLastSeed(*pExtraParams);

  SaveCache();
}

void MJLevelObject::SetCFG(
    const SCFG & _cfg
  )
{
  m_chipsTypes.clear();
  AddChipPairType(MJFirstDigitGroup,  _cfg.m_iFirstGroupDigitChipsNumber,  false);
  AddChipPairType(MJSecondDigitGroup, _cfg.m_iSecondGroupDigitChipsNumber, false);
  AddChipPairType(MJThirdDigitGroup,  _cfg.m_iThirdGroupDigitChipsNumber,  false);
  AddChipPairType(MJMysticGroup,      _cfg.m_iMysticGroupChipsNumber,      false);
  AddChipPairType(MJDragonGroup,      _cfg.m_iDragonChipGroupNumber,       false);
  AddChipPairType(MJWindGroup,        _cfg.m_iWindChipGroupNumber,         false);

  int flowerChipGroupNumber = _cfg.m_iFlowerChipGroupNumber;
  int seasonChipGroupNumber = _cfg.m_iSeasonChipGroupNumber;

  if (m_type != EAlgorithmType::algorithm_type_count)
  {
    flowerChipGroupNumber = std::min(1, flowerChipGroupNumber);
    seasonChipGroupNumber = std::min(1, seasonChipGroupNumber);
  }

  AddChipPairType(MJFlowerGroup, flowerChipGroupNumber, true);
  AddChipPairType(MJSeasonGroup, seasonChipGroupNumber, true);
}

void MJLevelObject::SetRepeatTypeTimes(
    int _count
  )
{
  m_randomiser.SetRepeatTypeTimes(_count);
}

bool MJLevelObject::BuildWithSeed(
    int _seed
  )
{
  RestoreFromCache();
  for (MJChip & chip: m_levelChips)
  {
    chip.SetTypeValue(INITAL_CHIP_TYPE_VALUE);
    chip.SetType(MJChipType::MJChipTypeStandard);
  }

  if (!m_randomiser.RandomizeChipsWithSeed(
        m_levelChips,
        m_chipsTypes,
        m_firstGoldenChipPositions,
        m_secondGoldenChipPositions,
        _seed))
  {
    return false;
  }

  m_activeChips.clear();
  for (MJChip & chip: m_levelChips)
  {
    if (!m_activeChips.count(chip.GetTypeValue()))
      m_activeChips[chip.GetTypeValue()] = std::set<MJChip *>();

    TryToAddActiveChips(&chip);
  }

  SaveCache();
  return true;
}

void MJLevelObject::SaveCache()
{
  m_activeChipsCache = m_activeChips;
  for (MJChip & chip: m_levelChips)
    chip.SaveNeighborsInCache();
}

void MJLevelObject::RestoreFromCache()
{
  m_activeChips = m_activeChipsCache;
  for (MJChip & chip: m_levelChips)
    chip.ResoreNeighborsFromCache();
}

void MJLevelObject::Clear()
{
  m_levelChips.clear();
  m_activeChips.clear();
  m_activeChipsCache.clear();
  m_firstGoldenChipPositions.clear();
  m_secondGoldenChipPositions.clear();
}

void MJLevelObject::AddChipPairType(
    MJChipGroup _group,
    int         _count,
    bool        _useRandom
  )
{
  std::string value1;

//  Original code from mapg:
//  ========================
//  for (int i = 0; i < _count; ++i)
//  {
//    const std::string &prefix = ChipUtils::GetGroupPrefix(_group);
//    value1 = prefix + std::to_string(i);
//    if (_useRandom)
//      value2 = prefix + std::to_string(std::rand() % _count);
//    else
//      value2 = prefix + std::to_string(i);
//
//    m_chipsTypes.push_back({value1, value1});
//  }

  for (int i = 0; i < _count; ++i)
  {
    const std::string & prefix = ChipUtils::GetGroupPrefix(_group);
    if (_useRandom)
      value1 = prefix + '0';
    else
      value1 = prefix + std::to_string(i);
    m_chipsTypes.push_back({value1, value1});
  }
}

void MJLevelObject::TryToAddActiveChips(
    MJChip * _chip
  )
{
  if (_chip->IsBlockedByNeighbors())
    return;

  const std::string & typeValue = _chip->GetTypeValue();
  std::map<std::string, std::set<MJChip *>>::iterator it = m_activeChips.find(typeValue);
  if (it == m_activeChips.end())
    throw MJToolException("Illegal state: TryToAddActiveChips(). it == m_activeChips.end()");
  it->second.insert(_chip);
}

void MJLevelObject::RemoveFromActiveChipsAndUpdate(
    MJChip * _chip
  )
{
  const std::string & typeValue = _chip->GetTypeValue();
  std::map<std::string, std::set<MJChip *>>::iterator it = m_activeChips.find(typeValue);
  if (it == m_activeChips.end())
    throw MJToolException("Illegal state: RemoveFromActiveChips() it == m_activeChips.end()");
  it->second.erase(_chip);

  for (MJChip * chip: _chip->GetNeighbors(Top))
    TryToAddActiveChips(chip);
  for (MJChip * chip: _chip->GetNeighbors(Bottom))
    TryToAddActiveChips(chip);
  for (MJChip * chip: _chip->GetNeighbors(Left))
    TryToAddActiveChips(chip);
  for (MJChip * chip: _chip->GetNeighbors(Right))
    TryToAddActiveChips(chip);
}

std::pair<MJChip *, MJChip *> MJLevelObject::GetActivePair()
{
  m_activeTypes.clear();

  for (std::map<std::string, std::set<MJChip *>>::iterator it = m_activeChips.begin();
       it != m_activeChips.end();
       ++it)
  {
    if (it->second.size() >= 2)
      m_activeTypes.push_back(it);
  }

  if (m_activeTypes.empty())
    return {nullptr, nullptr};

  size_t typeIndex = LocalRandomInInterval(0, m_activeTypes.size() - size_t(1));
  std::set<MJChip *> & types = m_activeTypes[typeIndex]->second;
  size_t firstIndex  = LocalRandomInInterval(0, types.size() - size_t(1));
  size_t secondIndex = LocalRandomInInterval(0, types.size() - size_t(2));

  MJChip * pFirst = nullptr;
  MJChip * pSecond = nullptr;

  std::set<MJChip *>::iterator firstIt = types.begin();
  std::advance(firstIt, firstIndex);
  pFirst = *firstIt;
  types.erase(firstIt);

  std::set<MJChip *>::iterator secondIt = types.begin();
  std::advance(secondIt, secondIndex);
  pSecond = *secondIt;
  types.erase(secondIt);

  return {pFirst, pSecond};
}
