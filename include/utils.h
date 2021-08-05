#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>

/***********/
/* Utils */
/***********/
namespace Utils
{
  // convert string to digit
  bool is_digit(const std::string str);

  // tokenize line
  std::vector<std::string> tokens(std::string line, const char delim = ' '); 
  
  // get first token
  std::string first(const std::vector<std::string> &tokens);

  // get all except first token
  std::vector<std::string> remove_first(const std::vector<std::string> &tokens);
};
#endif
