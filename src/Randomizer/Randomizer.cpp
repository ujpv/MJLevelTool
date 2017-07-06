#include "LevelObject/Chip.h"
#include "Utils/Constants.h"
#include "Randomizer.h"
#include "Randomizer/RandomWrapper.h"

void MJRandomizer::SetType(
    EAlgorithmType _type
  )
{
  m_type = _type;
  if (_type == EAlgorithmType::algorithm_type_count)
    m_randomWrapper.SetType(CRandomWrapper::eRandomType::stdrandom);
  else
    m_randomWrapper.SetType(CRandomWrapper::eRandomType::match3random);
}
