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
#include "LevelObject/LevelObject.h"

//class MJChip;

class MJRandomizer
{
public:
  MJRandomizer()
    : m_type(EAlgorithmType::regular_type)
  {}

  void SetType(
      EAlgorithmType _type
    );

private:
  EAlgorithmType m_type;
  CRandomWrapper m_randomWrapper;
};
#endif // RANDOMIZER_H
