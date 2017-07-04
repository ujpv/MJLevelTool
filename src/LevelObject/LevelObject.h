#ifndef LEVELOBJECT_H
#define LEVELOBJECT_H

#include "Randomizer/Randomizer.h"
#include "Chip.h"

class MJLevelObject
{
public:
  MJLevelObject();
  void initWithDictionary(
      const std::string & _levelFileName
    );

private:
  void Clear();

private:
  MJRandomizer        m_randomiser;
  std::vector<MJChip> m_levelChips;
  std::set<MJChip *>  m_startChips;
};

#endif // LEVELOBJECT_H
