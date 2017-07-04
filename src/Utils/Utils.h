#ifndef UTILS_H
#define UTILS_H

#include <Plist.hpp>

struct SPoint2d
{
  float x;
  float y;
};

namespace PlistUtils
{
  bool getIntForKey(
      const Plist::dictionary_type & _dict,
      const std::string &            _key,
      int &                          _value
      );

  bool getStringForKey(
      const Plist::dictionary_type & _dict,
      const std::string &            _key,
      std::string &                  _value
    );

  const Plist::dictionary_type * getDictForKey(
      const Plist::dictionary_type & _dict,
      const std::string &            _key
    );

  const Plist::array_type * getArrayForKey(
      const Plist::dictionary_type & _dict,
      const std::string &            _key
    );

  const Plist::array_type * getArrayForIndex(
      const Plist::array_type & _array,
      int                       _index
    );
}

#endif // UTILS_H
