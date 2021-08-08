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
#include<stack>

/***********/
/* Utils */
/***********/
namespace Utils
{
  /* Tell if string is digit */
  bool is_digit(const std::string str);

  /* Tell if string is alphabetic */
  bool is_alphabetic(const std::string str);

  /* Tokenize line */
  std::vector<std::string> tokens(std::string line, const char delim = ' '); 
  
  /* Get first token */
  std::string first(const std::vector<std::string> &tokens);

  /* Get all tokens except first one */
  std::vector<std::string> remove_first(const std::vector<std::string> &tokens);

  /* Convert expression from infix to postfix */
  std::string to_postfix(const std::string infix);
};
#endif
