#include "Exceptions/Exceptions.h"
#include "LevelObject.h"
#include "Utils/Constants.h"

static const std::string kChipLayers = "Layers";;

MJLevelObject::MJLevelObject()
{}

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

      if (chip.GetTypeValue().find(kGoldChipGroupPrefix) == std::string::npos)
        chip.SetTypeValue("g0");
    }
  }

  for (MJChip & chip: m_levelChips)
    chip.SetupNeighbors(m_levelChips);
}

void MJLevelObject::Clear()
{
  m_startChips.clear();
  m_levelChips.clear();
}
