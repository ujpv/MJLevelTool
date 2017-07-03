#include "exceptions.h"

MJToolException::MJToolException(
    const std::string & _what_arg
  ): std::exception(_what_arg.c_str())
{
}
