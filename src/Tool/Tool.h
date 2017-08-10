#ifndef TOOL_H
#define TOOL_H

#include "Config/ToolConfig.h"
#include "Utils/Constants.h"
#include "Utils/Utils.h"

void EstimateComplexity(
    const std::vector<std::string> & _fileNames
  );

void FindSeeds(
    const std::vector<std::string> & _fileNames
  );

#endif // TOOL_H
