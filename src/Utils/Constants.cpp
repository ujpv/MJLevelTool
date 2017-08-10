#include "Constants.h"

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

const std::string INITAL_CHIP_TYPE_VALUE = "g0";

const std::string &ChipUtils::GetGroupPrefix(
    MJChipGroup _group
  )
{
  std::map<MJChipGroup, std::string>::iterator it = chipGroupPrefix.find(_group);
  if (it != chipGroupPrefix.end())
    return it->second;

  return chipGroupPrefix[MJGoldChipGroup];
}
