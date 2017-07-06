#include "Constants.h"

//const std::string kGoldChipGroupPrefix = "g";
//const std::string kDigitChipFirstGroupPrefix = "b";
//const std::string kDigitChipSecondGroupPrefix = "c";
//const std::string kDigitChipThirdGroupPrefix = "t";
//const std::string kMysticChipGroupPrefix = "m";
//const std::string kDragonChipGroupPrefix = "d";
//const std::string kWindChipGroupPrefix = "w";
//const std::string kFlowerChipGroupPrefix = "f";
//const std::string kSeasonChipGroupPrefix = "s";

static std::map<MJChipGroup, std::string> chipGroupPrefix {
  {MJGoldChipGroup,     "g"},
  {MJFirstDigitGroup,   "b"},
  {MJSecondDigitGroup,  "c"},
  {MJThirdDigitGroup,   "t"},
  {MJMysticGroup,       "m"},
  {MJDragonGroup,       "d"},
  {MJWindGroup,         "w"},
  {MJFlowerGroup,       "f"},
  {MJSeasonGroup,       "s"},
};

const std::string &ChipUtils::GetGroupPrefix(
    MJChipGroup _group
  )
{
  std::map<MJChipGroup, std::string>::iterator it = chipGroupPrefix.find(_group);
  if (it != chipGroupPrefix.end())
    return it->second;

  return chipGroupPrefix[MJGoldChipGroup];
}
