#include <cstring>
#include <cstdint>
#include <iostream>

#include "Match3Random.h"

//
// Presets from kdplatform.h
//

#define KDUINT32_MAX 0xffffffffU
typedef std::int64_t KDint64;

//
// Constants
//

const unsigned short CMatch3Random::RandomMult[] = {
  0xe66d,
  0xdeec,
  0x0005
};


//
// Interface
//

void CMatch3Random::SetSeed(
    int _Seed
  )
{
  const unsigned int Seed = (unsigned int)(_Seed % KDUINT32_MAX);

  m_RandomSeed[0] = RandomSeed;
  m_RandomSeed[1] = (unsigned short) Seed;
  m_RandomSeed[2] = (unsigned short) ((unsigned long)Seed >> 16);
}

void CMatch3Random::SetRandomSeedValue(
    const unsigned short * _RandomSeed
  )
{
  std::memcpy(m_RandomSeed, _RandomSeed, RandomSeedLength);
}

const unsigned short * CMatch3Random::GetRandomSeedValue() const
{
  return m_RandomSeed;
}

int CMatch3Random::GetRandomNumber(
    int _Range
  )
{
  if (_Range > 0)
    return int((GetRandomNumber() * (KDint64)_Range) / ((KDint64)RandomMax + 1));

  return 0;
}

real CMatch3Random::GetRandomValue(
    real _From,
    real _To
  )
{
  real Random = GetRandomNumber() / (real)RandomMax;
  return _From + (_To - _From) * Random;
}

int CMatch3Random::GetRandomValue(
    int _from,
    int _to
  )
{
  int rangeDiff = _to - _from;
  int randNumber = GetRandomNumber();
  if (rangeDiff == INT32_MAX)
  {
    std::cerr << "Not valid upper bound!\r\n";
    return randNumber;
  }

  return _from + randNumber % (rangeDiff + 1);
}

//
// Service
//

int CMatch3Random::GetRandomNumber()
{
  unsigned int   accu;
  unsigned short temp[2];

  accu = (unsigned int) RandomMult[0] * (unsigned int) m_RandomSeed[0] +
         (unsigned int) RandomAdd;

  temp[0] = (unsigned short) accu;  /* lower 16 bits */
  accu >>= sizeof(unsigned short) * 8;
  accu += (unsigned int) RandomMult[0] * (unsigned int) m_RandomSeed[1] +
          (unsigned int) RandomMult[1] * (unsigned int) m_RandomSeed[0];
  temp[1] = (unsigned short) accu;  /* middle 16 bits */
  accu >>= sizeof(unsigned short) * 8;
  accu += RandomMult[0] * m_RandomSeed[2] + RandomMult[1] * m_RandomSeed[1] + RandomMult[2] * m_RandomSeed[0];

  m_RandomSeed[0] = temp[0];
  m_RandomSeed[1] = temp[1];
  m_RandomSeed[2] = (unsigned short) accu;

  return (int)((unsigned int) m_RandomSeed[2] << 15) +
              ((unsigned int) m_RandomSeed[1] >> 1);
}
