#ifndef __CALCULATOR_HPP
#define __CALCULATOR_HPP

#include <string>

// this function should expect that `expression` has already been
// validated and thus throw no exceptions
double parse_number(const std::string &expression);
std::string add_doubles_as_strings(const std::string &a, const std::string &b);

#endif // __CALCULATOR_HPP
