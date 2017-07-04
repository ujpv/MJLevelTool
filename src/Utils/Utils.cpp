#include "Utils.h"

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

const Plist::array_type *PlistUtils::getArrayForKey(
    const Plist::dictionary_type & _dict,
    const std::string &            _key
  )
{
  Plist::dictionary_type::const_iterator it = _dict.find(_key);
  if (it == _dict.end())
    return nullptr;

  return boost::any_cast<Plist::array_type>(&it->second);
}

const Plist::array_type * PlistUtils::getArrayForIndex(
    const Plist::array_type & _array,
    int                       _index
  )
{
  return boost::any_cast<Plist::array_type>(&_array[_index]);
}
