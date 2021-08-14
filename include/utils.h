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

#include "error.h"

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
  
  /* Get all tokens except first one */
  std::vector<std::string> remove_first_token(const std::vector<std::string> &tokens);
  
  /* Remove all char occurrences from string */
  void strip_char(const char c, std::string &str);

  /* Evaluate conditional expression */
  bool eval_conditional_expr(const std::string expr);
};
#endif
