#include <unordered_map>

#include "Tool.h"
#include "LevelObject/LevelObject.h"
#include "Exceptions/Exceptions.h"

static const std::string kExtraParams = "ExtraParams";
static const std::string kPresets     = "Presets";
static const std::string kLastSeed    = "LastSeed";

static const std::string kDigitChipFirstGroupNumber  = "DigitChipFirstGroupNumber";
static const std::string kDigitChipSecondGroupNumber = "DigitChipSecondGroupNumber";
static const std::string kDigitChipThirdGroupNumber  = "DigitChipThirdGroupNumber";
static const std::string kDragonChipGroupNumber      = "DragonChipGroupNumber";
static const std::string kFlowerChipGroupNumber      = "FlowerChipGroupNumber";
static const std::string kMysticChipGroupNumber      = "MysticChipGroupNumber";
static const std::string kSeasonChipGroupNumber      = "SeasonChipGroupNumber";
static const std::string kWindChipGroupNumber        = "WindChipGroupNumber";
static const std::string kSeed                       = "Seed";
static const std::string kAlgorithm                  = "Algorithm";
static const std::string kL1                         = "L1";
static const std::string kEnabled                    = "Enabled";

void PrintProgress(
    const std::string & _message,
    const std::string & _fileName,
    double              _progress
  )
{
  static int lastProgess = -1;
  int progess = int(_progress * 100);
  if (lastProgess == progess)
    return;

  lastProgess = progess;
  std::cout << "\r" << _message << " " << _fileName << "..." << int(_progress * 100) << '%';
}

void PrintFinish(
    const std::string & _message,
    const std::string & _fileName
  )
{
  std::cout << "\r" << _message << ' ' << _fileName << "...OK   \n";
}

void WriteReport(
    const std::vector<size_t> & _results,
    const std::string         & _fileName,
    size_t                      _startSeed,
    size_t                      _seedCount,
    size_t                      _limit
  )
{
  CToolConfig & config = CToolConfig::Instance();
  const std::vector<std::pair<double, double>> & ranges = config.GetComplexityRanges();
  std::ofstream outfile;
  std::string fullFileName = config.GetOutPath() + GetPathSeparator() + config.GetReportFileName();
  outfile.open(fullFileName, std::ios_base::app);
  outfile.imbue(std::locale(""));

  outfile << "Level id" << ';' << "Range" <<';' << "Fails count" << ';' << "Start seed" << ';' << "Seed cound" << ';' <<  "Limit" << '\n';

  double rangeStep = 1.0 / _results.size();
  double curruntRange = 0.0;

  for (size_t i = 0; i < _results.size(); ++i)
  {
    //      << "Level id" << ';' << "Range min"                                  << ';' << "Fails count"                    << ';' << "Start seed" << ';' << "Seed cound" << ';' << "Limit" << '\n';
    outfile << _fileName  << ';' << ranges[i].first << "..." << ranges[i].second << ';' << double(_results[i]) / _seedCount << ';' << _startSeed   << ';' << _seedCount   << ';' << _limit  << '\n';
    curruntRange += rangeStep;
  }

  outfile.close();
  if (!outfile)
    throw MJToolException("Can't write file: " + fullFileName);
}

void EstimateComplexity(
    const std::vector<std::string> & _fileNames
  )
{
  const CToolConfig & config = CToolConfig::Instance();

  const size_t        limit =                             config.GetEstimateComplexityLimit();
  const size_t        seedCount =                         config.GetEstimateComplexitySeedCount();
  const std::string & levelsPath =                        config.GetLevelsPath();
  const std::vector<std::pair<double, double>> & ranges = config.GetComplexityRanges();

  const double maxFailsCount = std::max_element(
              ranges.begin(),
              ranges.end(),
              [] (const std::pair<double, double> & _lhs, const std::pair<double, double> & _rhs) {
                return _lhs.second < _rhs.second;
            })->second;

  const std::map<std::string, SCFG> & CFGs       = config.GetCGFs();
  const std::vector<std::string>    & namesOfCFG = config.GetNamesOfCGF();

  for (const std::string & fileName: _fileNames)
  {
    try
    {
      std::vector<size_t> results(ranges.size(), 0);
      size_t seedsRemains = seedCount;
      MJLevelObject level;
      level.InitWithDictionary(levelsPath + GetPathSeparator() + fileName);
      size_t startSeed = 0;
      size_t currentSeed = startSeed;

      while (seedsRemains != 0)
      {
        for (const std::string & cfgName: namesOfCFG)
        {
          PrintProgress("Estimating", fileName, static_cast<double>(seedCount - seedsRemains) / seedCount);

          level.SetCFG(CFGs.at(cfgName));
          if (!level.BuildWithSeed(static_cast<int>(currentSeed)))
            continue;

          double fails = level.PlayRandomNTimes(limit);
          if (fails > maxFailsCount)
            continue;

          for (size_t i = 0; i < ranges.size(); ++i)
          {
            if (ranges[i].first <= fails && fails <= ranges[i].second)
              ++results[i]; 
          }

          --seedsRemains;
          if (seedsRemains == 0)
            break;
        }
        ++currentSeed;
      }
      WriteReport(results, fileName, startSeed, seedCount, limit);
      PrintFinish("Estimating", fileName);
    }
    catch(std::exception & e)
    {
      std::cerr << (std::string("\nError: ") + fileName + ". Reason: " + e.what()) << std::endl;
    }
  }
}

void SavePlistWithPresets(
    const std::string &                        _plistFileName,
    const std::vector<std::pair<SCFG, size_t>> _presets,
    size_t                                     _lastSeed
  )
{
  // Inital data
  CToolConfig & config = CToolConfig::Instance();

  const std::string & levelPath = config.GetLevelsPath();
  const std::string & outPath   = config.GetOutPath();

  const std::string inputFile  = levelPath + GetPathSeparator() + _plistFileName;
  const std::string outputFile = outPath   + GetPathSeparator() + _plistFileName;

  // Read level plist
  Plist::dictionary_type levelData;
  try
  {
    Plist::readPlist(inputFile.c_str(), levelData);
  }
  catch (Plist::Error & e)
  {
    throw MJToolException("Can't read " + inputFile + ". Reason: \n" + e.what());
  }

  Plist::dictionary_type * pExtraParams = PlistUtils::getDictForKeyOrCreate(levelData, kExtraParams);
  if (!pExtraParams)
    throw MJToolException("Illegal state. PlistUtils::getDictForKeyOrCreate(levelData, kExtraParams)");

  Plist::array_type * pPresets = PlistUtils::getArrayForKeyOrCreate(*pExtraParams, kPresets);
  if (!pPresets)
    throw MJToolException("Illegal state. PlistUtils::getArrayForKeyOrCreate(levelData, kPresets)");

  // Save seed in plist
  (*pExtraParams)[kLastSeed] = Plist::integer_type(_lastSeed);
  (*pExtraParams)[kEnabled] = Plist::integer_type(1); // Use seed algo
  for (const std::pair<SCFG, size_t> & preset: _presets)
  {
    Plist::dictionary_type presetDict;
    presetDict[kAlgorithm] =                  Plist::string_type(kL1);
    presetDict[kSeed] =                       Plist::string_type(std::to_string(preset.second));
    presetDict[kDigitChipFirstGroupNumber]  = Plist::integer_type(preset.first.m_iFirstGroupDigitChipsNumber);
    presetDict[kDigitChipSecondGroupNumber] = Plist::integer_type(preset.first.m_iSecondGroupDigitChipsNumber);
    presetDict[kDigitChipThirdGroupNumber]  = Plist::integer_type(preset.first.m_iThirdGroupDigitChipsNumber);
    presetDict[kDragonChipGroupNumber] =      Plist::integer_type(preset.first.m_iDragonChipGroupNumber);
    presetDict[kFlowerChipGroupNumber] =      Plist::integer_type(preset.first.m_iFlowerChipGroupNumber);
    presetDict[kMysticChipGroupNumber] =      Plist::integer_type(preset.first.m_iMysticGroupChipsNumber);
    presetDict[kSeasonChipGroupNumber] =      Plist::integer_type(preset.first.m_iSeasonChipGroupNumber);
    presetDict[kWindChipGroupNumber] =        Plist::integer_type(preset.first.m_iWindChipGroupNumber);

    pPresets->push_back(presetDict);
  }

  // Save plist to file
  try
  {
    Plist::writePlistXML(outputFile.c_str(), levelData);
  }
  catch (Plist::Error & e)
  {
    throw MJToolException("Can't write " + outputFile + ". Reason: \n" + e.what());
  }
}

void AppednSeedReport(
    const std::string & _fileName,
    const std::string & _cfg,
    size_t              _seed,
    size_t              _limit,
    double              _fails
  )
{
  CToolConfig & config = CToolConfig::Instance();

  std::stringstream ssFileName;
  ssFileName << config.GetOutPath() << GetPathSeparator() << _fileName << ".csv";
  std::string fullFileName = ssFileName.str();

  bool isFileExist = false;
  {
    std::ifstream file(fullFileName);
    isFileExist = !!file;
  }
  
  std::stringstream ssContent;
  ssContent.imbue(std::locale(""));

  if (!isFileExist)
    ssContent << "Level ID; Seed; CFG; Experiments count; Fails rate\n";
  //            Level ID     ;      Seed     ;     CFG    ; Experiments count ; Fails rate
  ssContent << _fileName << ';' << _seed << ';' << _cfg << ';' << _limit << ';' << _fails << '\n';

  std::ofstream file;
  file.open(fullFileName, std::ios_base::app);
  file << ssContent.str();

  file.close();
  if (!file)
    throw MJToolException("Can't append result to " + fullFileName);
}

void FindSeeds(
    const std::vector<std::string> & _fileNames
  )
{
  const CToolConfig & config = CToolConfig::Instance();

  const std::string & levelsPath = config.GetLevelsPath();
  const std::map<std::string, SCFG> & CFGs = config.GetCGFs();
  const std::vector<std::string> & namesOfCFGs = config.GetNamesOfCGF();
  static std::unordered_map<std::string, float> resultsCache;

  for (const std::string & fileName: _fileNames)
  {
    std::vector<std::pair<SCFG, size_t>> results;

    try
    {
      const SLevelParams & levelParams = config.GetLevelParams(fileName);
      const size_t seedNeed = levelParams.m_seedNeed;
      const size_t limit = levelParams.m_Limit;
      const double failsMin = levelParams.m_DifficultyMin;
      const double failsMax = levelParams.m_DifficultyMax;

      MJLevelObject level;
      level.InitWithDictionary(levelsPath + GetPathSeparator() + fileName);
      level.SetRepeatTypeTimes(1);

      size_t currentSeed = std::max(levelParams.m_startSeed, level.GetLastSeed());

      while (results.size() != seedNeed)
      {
        for (const std::string & nameOfCFG: namesOfCFGs)
        {
          PrintProgress("Search seeds", fileName, results.size() / static_cast<double>(seedNeed));

          std::stringstream ssCacheName;
          ssCacheName << levelsPath << '_' << currentSeed << '_' << nameOfCFG;
          std::string cacheName = ssCacheName.str();
          std::unordered_map<std::string, float>::const_iterator cacheIt = resultsCache.find(cacheName);

          const SCFG & cfg = CFGs.at(nameOfCFG);
          double fails = -1;

          if (cacheIt == resultsCache.end())
          {
            // Not calcutated before
            level.SetCFG(cfg);

            if (!level.BuildWithSeed(static_cast<int>(currentSeed)))
            {
              resultsCache[cacheName] = -1;
              continue;
            }

            fails = level.PlayRandomNTimes(limit);
            resultsCache[cacheName] = fails;
          }
          else
          if (cacheIt->second > 0)
          {
            // Calcutated before
            fails = cacheIt->second;
          }
          else
          { 
            // Calcutated before. Can't build
            continue;
          }

          try
          {
            AppednSeedReport(fileName, nameOfCFG, currentSeed, limit, fails);
          }
          catch (std::exception & e)
          {
            std::cerr << "\nError: " << e.what() << std::endl;
          }

          if (!(failsMin <= fails && fails <= failsMax))
            continue;

          results.push_back({cfg, currentSeed});
          if (results.size() == seedNeed)
            break;
        }
        ++currentSeed;
      }

      SavePlistWithPresets(fileName, results, currentSeed);
      PrintFinish("Search seeds", fileName);
    }
    catch(std::exception & e)
    {
      std::cerr << "\nError: " << fileName << ". Reason: " << e.what() << std::endl;
    }
  }
}
