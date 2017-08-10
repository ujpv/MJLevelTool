#include "Exceptions/Exceptions.h"

MJToolException::MJToolException(
    const std::string & _whatArg
  ): std::runtime_error(_whatArg)
{}

