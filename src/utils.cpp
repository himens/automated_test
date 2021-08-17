#include "utils.h"

namespace Utils
{
  /****************/
  /* Print banner */
  /****************/
  void print_banner(const std::string str, size_t length)
  {
    if (length == 0) length = str.size() + 2;

    std::cout << "\n";
    std::cout << std::string(length, '*') << "\n";
    std::cout << str << "\n";
    std::cout << std::string(length, '*') << "\n";
  }


  /********************************/
  /* Tell if string is alphabetic */
  /********************************/
  bool is_alphabetic(const std::string str)
  {
    return !str.empty() ? std::all_of(str.begin(), str.end(), ::isalpha) : false;
  }
  
  
  /***************************/
  /* Tell if string is digit */
  /***************************/
  bool is_digit(const std::string str)
  {
    return !str.empty() ? (str.find_first_not_of("0123456789.") == std::string::npos) : false;
  } 


  /*****************/
  /* Tokenize line */
  /*****************/
  std::vector<std::string> tokens(const std::string line, const char delim) 
  {
    std::vector<std::string> tokens;
    std::string token;
    auto first = line.find_first_of('"');
    auto last = line.find_last_of('"');

    // do not tokenize what's inside quotes
    if (first != std::string::npos && last != std::string::npos && delim != '"') 
    {
      std::istringstream iss(line.substr(0, first));
      while (std::getline(iss, token, delim)) 
      {
	if (!token.empty()) tokens.push_back(token);
      }

      tokens.push_back(line.substr(first, last));
    }

    // tokenize whole line
    else
    {
      std::istringstream iss(line);
      while (std::getline(iss, token, delim)) 
      {
	if (!token.empty()) tokens.push_back(token);
      }
    }

    return tokens;
  }

  /*******************************************/
  /* Remove all char occurrences from string */
  /*******************************************/
  void strip_char(const char c, std::string &str)
  {
    str.erase(std::remove(str.begin(), str.end(), c), str.end()); 
  }

  /***********************************/
  /* Get all tokens except first one */
  /***********************************/
  std::vector<std::string> remove_first_token(const std::vector<std::string> &tokens)
  {
    auto args = tokens;
    if (tokens.size() > 1) args.erase(args.begin());
    else args = {};

    return args;
  }
};
