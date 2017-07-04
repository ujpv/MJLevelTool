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

  m_zLayer = _zLayer;

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
  return m_zLayer;
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

    if (m_zLayer == chip.GetZOrder())
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
            (fDiffX <= 0.0f && fDiffX >= -2 * POSITION_ACCURACY) &&
            (
              (fDiffY1 <= POSITION_ACCURACY && fDiffY1 >= -POSITION_ACCURACY) ||
              (fDiffY2 <= POSITION_ACCURACY && fDiffY2 >= -POSITION_ACCURACY) ||
              (chipY - chip.GetPosition().y <= FLT_EPSILON)
            )
           )
        {
          m_rNeighborsRight.insert(&chip);
          chip.GetNeighbors(Left).insert(this);
        }
      }
    }
    else
    if (m_zLayer + 1 == chip.GetZOrder())
    {
      if (m_rNeighborsTop.size() < 4)
      {

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

  size_t nPosLeft  = value.find('{');
  size_t nPosRight = value.find('}');

  if (nPosLeft == std::string::npos || nPosRight == std::string::npos || nPosLeft > nPosRight)
    return false;

  value = value.substr(nPosLeft + 1, nPosRight - nPosLeft - 1);

  std::stringstream ss(value);
  std::string trash;

  float x = 0;
  float y = 0;

  ss >> x;
  if (ss.fail())
    return false;
  ss >> trash;
  if (ss.fail() || trash != ",")
    return false;
  ss >> y;
  if (ss.fail())
    return false;

  m_position.x = x;
  m_position.y = y;
  return true;
}
