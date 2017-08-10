#ifndef UTILS_H
#define UTILS_H

#include <Plist.hpp>

struct SPoint2d
{
  float x;
  float y;
  bool equals(
      const SPoint2d & _other
    ) const;
};

std::ostream & operator<<(
    std::ostream &   _stream,
    const SPoint2d & _point
  );

inline char GetPathSeparator()
{
#ifdef _WIN32
    return '\\';
#else
    return '/';
#endif
}

bool ParsePoint2d(
    const std::string & _array,
    SPoint2d &          _point
  );

size_t LocalRandomInInterval(
    size_t _min,
    size_t _max
  );

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

  bool getStringForIndex(
      const Plist::array_type & _array,
      int                       _index,
      std::string &             _value
    );

  const Plist::dictionary_type * getDictForKey(
      const Plist::dictionary_type & _dict,
      const std::string &            _key
    );

  Plist::dictionary_type * getDictForKeyOrCreate(
      Plist::dictionary_type & _dict,
      const std::string &      _key
    );

  Plist::array_type * getArrayForKeyOrCreate(
      Plist::dictionary_type & _dict,
      const std::string &      _key
    );

  const Plist::array_type * getArrayForKey(
      const Plist::dictionary_type & _dict,
      const std::string &            _key
    );

  const Plist::array_type * getArrayForIndex(
      const Plist::array_type & _array,
      int                       _index
    );

  const Plist::dictionary_type * getDictForIndex(
      const Plist::array_type & _array,
      int                       _index
    );
}

#endif // UTILS_H
