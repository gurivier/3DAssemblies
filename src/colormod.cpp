#include "colormod.hpp"



std::ostream& Color::operator<<(std::ostream& os, Code code)
{
  return os << "\033[1;" << static_cast<int>(code) << "m";
}

/*

Color::Modifier::Modifier (Code pCode)
  : code(pCode)
{
  // Nothing
}

friend std::ostream& Color::operator<<(std::ostream& os, const Modifier& mod)
{
  return os << "\033[1;" << mod.code << "m" ;
}

*/

