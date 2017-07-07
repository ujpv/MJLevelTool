#ifndef CONSTANTS_H
#define CONSTANTS_H
#include <string>
#include <map>

//extern const std::string kDigitChipFirstGroupPrefix;
//extern const std::string kDigitChipSecondGroupPrefix;
//extern const std::string kDigitChipThirdGroupPrefix;
//extern const std::string kDragonChipGroupPrefix;
//extern const std::string kFlowerChipGroupPrefix;
//extern const std::string kGoldChipGroupPrefix;
//extern const std::string kMysticChipGroupPrefix;
//extern const std::string kSeasonChipGroupPrefix;
//extern const std::string kWindChipGroupPrefix;

extern const std::string INITAL_CHIP_TYPE_VALUE;
enum MJChipGroup
{
  MJGoldChipGroup,
  MJFirstDigitGroup,
  MJSecondDigitGroup,
  MJThirdDigitGroup,
  MJMysticGroup,
  MJDragonGroup,
  MJWindGroup,
  MJFlowerGroup,
  MJSeasonGroup
};

enum EAlgorithmType
{
  regular_type,
  random_type,

  algorithm_type_count
};


namespace ChipUtils
{
  const std::string & GetGroupPrefix(
      MJChipGroup _group
    );
}

#endif // CONSTANTS_H
