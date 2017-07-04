#ifndef MATCH3RANDOM_H
#define MATCH3RANDOM_H

typedef float real;

class CMatch3Random
{
public: // Smart pointer

  CMatch3Random() {}
  ~CMatch3Random() {}

public: // Interface

  void SetSeed(
      int _Seed
    );

  void SetRandomSeedValue(
      const unsigned short * _RandomSeed
    );

  const unsigned short * GetRandomSeedValue() const;

  int GetRandomNumber(
      int _Range
    );

  real GetRandomValue(
      real _From,
      real _To
    );

  int GetRandomValue(
      int _from,
      int _to
    );

protected: // Service

  int GetRandomNumber();

protected: // Constants

  static const int            RandomSeedLength = 3;
  static const int            RandomMax        = 0x7fffffff;
  static const unsigned short RandomSeed       = 0x330e;
  static const unsigned short RandomAdd        = 0x000b;

  static const unsigned short RandomMult[RandomSeedLength];


protected: // Attributes

  unsigned short m_RandomSeed[RandomSeedLength];
};

#endif // MATCH3RANDOM_H
