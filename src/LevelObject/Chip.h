#ifndef CHIP_H
#define CHIP_H

#include <set>
#include <Plist.hpp>

enum MJChipType
{
  MJChipTypeStandard = 0,
  MJChipTypeFire,
  MJChipTypeWind,
  MJChipTypeWater,
  MJChipTypeGolden,
  MJChipTypeUnknown,
};

class MJChip
{
public:
    MJChip();
    bool InitWithParameters

private:
    std::set<MJChip *> m_rNeighborsTop;
    std::set<MJChip *> m_rNeighborsLeft;
    std::set<MJChip *> m_rNeighborsRight;
    std::set<MJChip *> m_rNeighborsBottom;

    MJChipType  m_eChipType;
    std::string m_eChipTypeValue;

    struct
    {
      float x; float y;
    } m_position;
};

#endif // CHIP_H
