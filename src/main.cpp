#include <iostream>
#include <stdexcept>
#include <chrono>

#include <Plist.hpp>

#include "Config/ToolConfig.h"
#include "Tool/Tool.h"

using namespace std;

#ifdef DEBUG
const std::string gConfigPath = "d:\\develop\\MJLevelTool\\latest_build\\config.json";
#else
const std::string gConfigPath = "config.json";
#endif

inline void ExitMassage()
{
  std::cout << "\nPress any key for exit...\n";
  std::getchar();
}

int main()
{
  using namespace std::chrono;
  high_resolution_clock::time_point startTime = high_resolution_clock::now();

  CToolConfig & config = CToolConfig::Instance();
  std::cout << "Loadin config...";
  try
  {
    config.Init(gConfigPath);
  }
  catch (std::exception & e)
  {
    std::cout << "\nCan't read config. Reason: " << e.what() << std::endl;
    ExitMassage();
    return 0;
  }
  std::cout << "\rLoadin config...OK\n";

  if (config.IsOnlyEstimateComplexity())
    EstimateComplexity(config.GetLevelsForCheck());
  else
  {
    size_t pathCount = config.GetPathsCount();
    for (size_t i = 0; i < pathCount; ++i) 
    {
      config.SetCurrentPathIndex(i);
      std::cout << "Calculating leves set #" << i << ":\n" 
                << "Levelpath: " << config.GetLevelsPath() << '\n' 
                << "OutPath:"    << config.GetOutPath() << '\n';
      FindSeeds(config.GetLevelsForCheck());
    }
  }

  std::cout << "Execution time: " << duration_cast<milliseconds>(high_resolution_clock::now() - startTime).count() << " ms\n";
  ExitMassage();
  return 0;
}
