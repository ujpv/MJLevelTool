#include "Exceptions/Exceptions.h"
#include "LevelObject.h"
#include "Utils/Constants.h"

static const std::string kChipLayers = "Layers";
static const std::string kFirstGoldenChipPosition = "FirstGoldenChipPosition";
static const std::string kSecondGoldenChipPosition = "SecondGoldenChipPosition";
static const std::string kMetadata = "Metadata";

MJLevelObject::MJLevelObject()
  : m_Type(EAlgorithmType::regular_type)
{
  m_randomiser.SetType(m_Type);
}

void MJLevelObject::initWithDictionary(
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

  m_startChips.clear();
  m_levelChips.clear();

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

      m_startChips.insert(&chip);

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
}

void MJLevelObject::SetCFG(
    const SCFG & _cfg
  )
{
  AddChipPairType(MJFirstDigitGroup,  _cfg.m_iFirstGroupDigitChipsNumber,  false);
  AddChipPairType(MJSecondDigitGroup, _cfg.m_iSecondGroupDigitChipsNumber, false);
  AddChipPairType(MJThirdDigitGroup,  _cfg.m_iThirdGroupDigitChipsNumber,  false);
  AddChipPairType(MJMysticGroup,      _cfg.m_iMysticGroupChipsNumber,      false);
  AddChipPairType(MJDragonGroup,      _cfg.m_iDragonChipGroupNumber,       false);
  AddChipPairType(MJWindGroup,        _cfg.m_iWindChipGroupNumber,         false);

  int flowerChipGroupNumber = _cfg.m_iFlowerChipGroupNumber;
  int seasonChipGroupNumber = _cfg.m_iSeasonChipGroupNumber;

  if (m_Type != EAlgorithmType::algorithm_type_count)
  {
    flowerChipGroupNumber = std::min(1, flowerChipGroupNumber);
    seasonChipGroupNumber = std::min(1, seasonChipGroupNumber);
  }

  AddChipPairType(MJFlowerGroup, _cfg.m_iFlowerChipGroupNumber, true);
  AddChipPairType(MJSeasonGroup, _cfg.m_iSeasonChipGroupNumber, true);
}

void MJLevelObject::BuildWithSeed(
    int _seed
  )
{
  m_randomiser.RandomizeChipsWithSeed(
        m_levelChips,
        m_chipsTypes,
        m_firstGoldenChipPositions,
        m_secondGoldenChipPositions,
        _seed);

//  std::vector<MJChip> &             _initialChips,
//  const std::vector<ChipTypePair> & _typesList,
//  const std::vector<SPoint2d> &     _firstGoldenChipPositions,
//  const std::vector<SPoint2d> &     _secondGoldenChipPositions,
//  int                               _seed)
}

void MJLevelObject::Clear()
{
  m_startChips.clear();
  m_levelChips.clear();
}

void MJLevelObject::AddChipPairType(
    MJChipGroup _group,
    int         _count,
    bool        _useRandom
  )
{
  std::string value1;
  std::string value2;

  for (int i = 0; i < _count; ++i)
  {
    const std::string &prefix = ChipUtils::GetGroupPrefix(_group);
    value1 = prefix + std::to_string(i);
    if (_useRandom)
      value2 = prefix + std::to_string(std::rand() % _count);
    else
      value2 = prefix + std::to_string(i);

    m_chipsTypes.push_back({value1, value2});
  }
}
