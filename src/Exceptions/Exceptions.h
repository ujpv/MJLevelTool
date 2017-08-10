#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>

class MJToolException : public std::runtime_error
{
public:
  MJToolException(
      const std::string & _whatArg
    );
};

#endif // EXCEPTIONS_H
