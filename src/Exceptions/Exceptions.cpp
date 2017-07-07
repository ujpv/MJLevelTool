#include "Exceptions/Exceptions.h"

MJToolException::MJToolException(
    const std::string & _what_arg
  ): std::runtime_error(_what_arg)
{}

