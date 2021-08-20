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
  /* Print banner */
  void print_banner(const std::string str);

  /* Tell if string is digit */
  bool is_digit(const std::string str);

  /* Tell if string is alphabetic */
  bool is_alphabetic(const std::string str);

  /* Tokenize line */
  std::vector<std::string> tokens(std::string line, const char delim = ' '); 
  
  /* Erase first element */
  void erase_front(std::vector<std::string> &vect);
  
  /* Remove all char occurrences from string */
  void strip_char(const char c, std::string &str);
};
#endif
