#include "Utils.h"
#include <sstream>
#include <cfloat>
#include <random>

bool PlistUtils::getIntForKey(
    const Plist::dictionary_type & _dict,
    const std::string &            _key,
    int &                          _value
  )
{
  Plist::dictionary_type::const_iterator it = _dict.find(_key);
  if (it == _dict.end())
    return false;

  const Plist::integer_type * pValue = boost::any_cast<Plist::integer_type>(&it->second);
  if (!pValue)
    return false;

  _value = static_cast<int>(*pValue);
  return true;
}

bool PlistUtils::getStringForKey(
    const Plist::dictionary_type & _dict,
    const std::string &            _key,
    std::string &                  _value
  )
{
  Plist::dictionary_type::const_iterator it = _dict.find(_key);
  if (it == _dict.end())
    return false;

  const Plist::string_type * pValue = boost::any_cast<Plist::string_type>(&it->second);
  if (!pValue)
    return false;

  _value = *pValue;
  return true;
}

const Plist::dictionary_type * PlistUtils::getDictForKey(
    const Plist::dictionary_type & _dict,
    const std::string &            _key
  )
{
  Plist::dictionary_type::const_iterator it = _dict.find(_key);
  if (it == _dict.end())
    return nullptr;

  return boost::any_cast<Plist::dictionary_type>(&it->second);
}

Plist::dictionary_type * PlistUtils::getDictForKeyOrCreate(
    Plist::dictionary_type & _dict,
    const std::string &      _key
  )
{
  Plist::dictionary_type::iterator it = _dict.find(_key);
  if (it != _dict.end())
  {
    Plist::dictionary_type * pResult = boost::any_cast<Plist::dictionary_type>(&it->second);
    if (pResult)
      return pResult;
  }

  _dict[_key] = Plist::dictionary_type();
  return boost::any_cast<Plist::dictionary_type>(&_dict[_key]);
}

const Plist::array_type * PlistUtils::getArrayForKey(
    const Plist::dictionary_type & _dict,
    const std::string &            _key
  )
{
  Plist::dictionary_type::const_iterator it = _dict.find(_key);
  if (it == _dict.end())
    return nullptr;

  return boost::any_cast<Plist::array_type>(&it->second);
}

Plist::array_type * PlistUtils::getArrayForKeyOrCreate(
    Plist::dictionary_type & _dict,
    const std::string &      _key
  )
{
  Plist::dictionary_type::iterator it = _dict.find(_key);
  if (it != _dict.end())
  {
    Plist::array_type * pResult = boost::any_cast<Plist::array_type>(&it->second);
    if (pResult)
      return pResult;
  }

  _dict[_key] = Plist::array_type();
  return boost::any_cast<Plist::array_type>(&_dict[_key]);
}

const Plist::array_type * PlistUtils::getArrayForIndex(
    const Plist::array_type & _array,
    int                       _index
  )
{
  return boost::any_cast<Plist::array_type>(&_array.at(static_cast<size_t>(_index)));
}

const Plist::dictionary_type * PlistUtils::getDictForIndex(
    const Plist::array_type & _array,
    int                       _index
  )
{
  return boost::any_cast<Plist::dictionary_type>(&_array.at(static_cast<size_t>(_index)));
}

bool ParsePoint2d(
    const std::string & _array,
    SPoint2d &          _point
  )
{
  size_t nPosLeft  = _array.find('{');
  size_t nPosRight = _array.find('}');

  if (nPosLeft == std::string::npos || nPosRight == std::string::npos || nPosLeft > nPosRight)
    return false;

  std::stringstream ss(_array.substr(nPosLeft + 1, nPosRight - nPosLeft - 1));
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

  _point.x = x;
  _point.y = y;
  return true;
}

bool PlistUtils::getStringForIndex(
    const Plist::array_type & _array,
    int                       _index,
    std::string &             _value
  )
{
  const Plist::string_type * pValue = boost::any_cast<Plist::string_type>(&_array[static_cast<size_t>(_index)]);
  if (!pValue)
    return false;

  _value = *pValue;
  return true;
}

bool SPoint2d::equals(
    const SPoint2d & _other
  ) const
{
  return (std::fabs(x - _other.x) < FLT_EPSILON)
      && (std::fabs(y - _other.y) < FLT_EPSILON);
}

std::ostream & operator<<(
    std::ostream &   _stream,
    const SPoint2d & _point
  )
{
  return _stream << '{' << _point.x << ", " << _point.y << "}";
}

size_t LocalRandomInInterval(
    size_t _min,
    size_t _max
  )
{
  static std::default_random_engine localGen(static_cast<unsigned>(time(nullptr)));
  std::uniform_int_distribution<size_t> distr(_min, _max);
  return distr(localGen);
}

