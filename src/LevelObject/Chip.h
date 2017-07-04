#ifndef CHIP_H
#define CHIP_H

#include <set>
#include <Plist.hpp>

#include "Utils/Utils.h"

enum MJChipType
{
  MJChipTypeStandard = 0,
  MJChipTypeFire,
  MJChipTypeWind,
  MJChipTypeWater,
  MJChipTypeGolden,
  MJChipTypeUnknown,
};

enum MJDirection
{
  Top,
  Bottom,
  Left,
  Right
};

class MJChip
{
public:
  MJChip();
  bool InitWithParameters(
      const boost::any * _params,
      int                _zLayer
    );

  void SetTypeValue(
      const std::string & _value
    );

  const std::string & GetTypeValue() const;

  MJChipType GetType() const;

  const SPoint2d & GetPosition() const;

  int GetZOrder() const;

  void SetupNeighbors(
      std::vector<MJChip> & _chips
    );

  std::set<MJChip *> & GetNeighbors(
      MJDirection _direction
    );

private:
  bool SetPosition(
      const Plist::dictionary_type * _params
    );

private:
  std::set<MJChip *> m_rNeighborsTop;
  std::set<MJChip *> m_rNeighborsLeft;
  std::set<MJChip *> m_rNeighborsRight;
  std::set<MJChip *> m_rNeighborsBottom;

  int         m_eChipType; // MJChipType
  std::string m_sChipTypeValue;
  std::string m_sId; // TODO!

  SPoint2d m_position;
  int      m_zLayer;
};

#endif // CHIP_H
