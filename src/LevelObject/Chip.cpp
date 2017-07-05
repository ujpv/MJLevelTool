#include <sstream>

#include "Exceptions/Exceptions.h"
#include "Chip.h"
#include <cmath>
#include <cfloat>

//static const std::string kChipLayers = "Layers";
//static const std::string kChipsLayerEdge = "ChipsLayerEdge";
static const std::string kChipType = "Type";
static const std::string kChipTypeValue = "TypeValue";
static const std::string kChipPosition = "Position";
//static const std::string kChipOffset = "Offset";
//static const std::string kDepth = "Depth";

static const float POSITION_ACCURACY = 0.1f;
static const float POSITION_CENTER =   0.5f;

MJChip::MJChip()
  : m_eChipType(MJChipTypeUnknown)
  , m_sChipTypeValue("g0")
{
}

bool MJChip::InitWithParameters(
    const boost::any * _params,
    int                _zLayer
  )
{
  const Plist::dictionary_type * params = boost::any_cast<Plist::dictionary_type>(_params);
  if (!params)
    return false;

  PlistUtils::getIntForKey(*params, kChipType, m_eChipType);
  PlistUtils::getStringForKey(*params, kChipTypeValue, m_sChipTypeValue);

  if (!SetPosition(params))
    return false;

  m_zOreder= _zLayer;

  return true;
}

void MJChip::SetTypeValue(
    const std::string & _value
    )
{
  m_sChipTypeValue = _value;
}

const std::string & MJChip::GetTypeValue() const
{
  return m_sChipTypeValue;
}

MJChipType MJChip::GetType() const
{
  return static_cast<MJChipType>(m_eChipType);
}

const SPoint2d &MJChip::GetPosition() const
{
  return m_position;
}

int MJChip::GetZOrder() const
{
  return m_zOreder;
}

void MJChip::SetupNeighbors(
    std::vector<MJChip> & _chips
  )
{
  m_rNeighborsBottom.clear();
  m_rNeighborsTop.clear();
  m_rNeighborsLeft.clear();
  m_rNeighborsRight.clear();

  float chipX = m_position.x;
  float chipY = m_position.y;

  for (MJChip & chip: _chips)
  {
    if (&chip == this)
      continue;

    if (m_zOreder == chip.GetZOrder())
    {
      float fDiffX  = chipX - chip.GetPosition().x;
      float fDiffY1 = chipY - chip.GetPosition().y - 0.5f;
      float fDiffY2 = chipY - chip.GetPosition().y + 0.5f;

      if (m_rNeighborsLeft.size() < 2)
      {
        if (
            (fDiffX <= 2 * POSITION_ACCURACY && fDiffX >= 0.0f) &&
            (
              (fDiffY1 <= POSITION_ACCURACY && fDiffY1 >= -POSITION_ACCURACY) ||
              (fDiffY2 <= POSITION_ACCURACY && fDiffY2 >= -POSITION_ACCURACY) ||
              (std::abs(chip.GetPosition().y - chipY) <= FLT_EPSILON)
            )
           )
        {
          m_rNeighborsLeft.insert(&chip);
          chip.GetNeighbors(Right).insert(this);
        }
      }

      if (m_rNeighborsRight.size() < 2)
      {
        if (
            (fDiffX >= -2 * POSITION_ACCURACY && fDiffX <= 0.0f) &&
            (
              (fDiffY1 <= POSITION_ACCURACY && fDiffY1 >= -POSITION_ACCURACY) ||
              (fDiffY2 <= POSITION_ACCURACY && fDiffY2 >= -POSITION_ACCURACY) ||
              (std::abs(chipY - chip.GetPosition().y) <= FLT_EPSILON)
            )
           )
        {
          m_rNeighborsRight.insert(&chip);
          chip.GetNeighbors(Left).insert(this);
        }
      }
    }
    else
    if (m_zOreder + 1 == chip.GetZOrder())
    {
      if (m_rNeighborsTop.size() < 4)
      {
        float fDiffX1 = chipX - chip.GetPosition().x - POSITION_CENTER;
        float fDiffX2 = chipX - chip.GetPosition().x + POSITION_CENTER;
        float fDiffY1 = chipY - chip.GetPosition().y - POSITION_CENTER;
        float fDiffY2 = chipY - chip.GetPosition().y + POSITION_CENTER;

        if (
            (
              (fDiffX1 <= POSITION_ACCURACY && fDiffX1 >= -POSITION_ACCURACY) ||
              (fDiffX2 <= POSITION_ACCURACY && fDiffX2 >= -POSITION_ACCURACY) ||
              (std::abs(chipX - chip.GetPosition().x) <= FLT_EPSILON)
            ) &&
            (
              (fDiffY1 <= POSITION_ACCURACY && fDiffY1 >= -POSITION_ACCURACY) ||
              (fDiffY2 <= POSITION_ACCURACY && fDiffY2 >= -POSITION_ACCURACY) ||
              (std::abs(chipY - chip.GetPosition().y) <= FLT_EPSILON)
            )
           )
        {
          m_rNeighborsTop.insert(&chip);
          chip.GetNeighbors(Bottom).insert(this);
        }
      }
    }
    else
    if (m_zOreder - 1 == chip.GetZOrder())
    {
      if (m_rNeighborsBottom.size() < 4)
      {
        float fDiffX1 = chipX - chip.GetPosition().x - POSITION_CENTER;
        float fDiffX2 = chipX - chip.GetPosition().x + POSITION_CENTER;
        float fDiffY1 = chipY - chip.GetPosition().y - POSITION_CENTER;
        float fDiffY2 = chipY - chip.GetPosition().y + POSITION_CENTER;

        if (
            (
              (fDiffX1 <= POSITION_ACCURACY && fDiffX1 >= -POSITION_ACCURACY) ||
              (fDiffX2 <= POSITION_ACCURACY && fDiffX2 >= -POSITION_ACCURACY) ||
              (std::abs(chipX  - chip.GetPosition().x) <= FLT_EPSILON)
             ) &&
            (
              (fDiffY1 <= POSITION_ACCURACY && fDiffY1 >= -POSITION_ACCURACY) ||
              (fDiffY2 <= POSITION_ACCURACY && fDiffY2 >= -POSITION_ACCURACY) ||
              (std::abs(chipY - chip.GetPosition().y) <= FLT_EPSILON)
            )
           )
        {
          m_rNeighborsBottom.insert(&chip);
          chip.GetNeighbors(Top).insert(this);
        }
      }
    }
  }
}

std::set<MJChip *> & MJChip::GetNeighbors(MJDirection _direction)
{
  switch (_direction) {
    case Bottom:
      return m_rNeighborsBottom;
    case Top:
      return m_rNeighborsTop;
    case Left:
      return m_rNeighborsLeft;
    case Right:
      return m_rNeighborsRight;
    default:
      throw MJToolException("MJChip::GetNeighbors. Illegal argument _dir=" + std::to_string(_direction));
  }
}

bool MJChip::SetPosition(
    const Plist::dictionary_type * _params
    )
{
  std::string value; // expected format - {x, y}
  if (!_params || !PlistUtils::getStringForKey(*_params, kChipPosition, value))
    return false;

  return ParsePoint2d(value, m_position);
}
