#ifndef RANDOMWRAPPER_H
#define RANDOMWRAPPER_H

#include <cstdint>
#include <random>

#include "Match3Random.h"

class CRandomWrapper
{
public:
  enum eRandomType
  {
    match3random,
    stdrandom,
    none
  };

  CRandomWrapper(
      eRandomType _type = match3random
    )
    : m_Type(_type)
  {}

  void SetSeed(
      int _seed
    )
  {
    if (m_Type == eRandomType::match3random)
    {
      m_Match3Random.SetSeed(_seed);
    }
    else if (m_Type == eRandomType::stdrandom)
    {
      m_StdRandom.seed(_seed);
    }
  }

  void SetType(
      eRandomType _type
    )
  {
    m_Type = _type;
  }

  std::int64_t GetRandomValue(
      int _min,
      int _max
    )
  {
    if (m_Type == eRandomType::match3random)
      return m_Match3Random.GetRandomValue(_min, _max);

    if (m_Type == eRandomType::stdrandom)
    {
      std::uniform_int_distribution<int> distr(_min, _max);
      return distr(m_StdRandom);
    }

    return 0;
  }

  std::default_random_engine m_StdRandom;
  CMatch3Random              m_Match3Random;
  eRandomType                m_Type;
};
#endif // RANDOMWRAPPER_H
