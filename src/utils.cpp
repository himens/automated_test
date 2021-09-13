#include "utils.h"

namespace Utils
{
  /****************/
  /* Print banner */
  /****************/
  void print_banner(const std::string str)
  {
    auto length = str.size() + 2;

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
    std::istringstream iss(line);

    while (std::getline(iss, token, delim)) 
    {
      // do not tokenize quotes "..."
      if (token.find('"') != std::string::npos)
      {
        auto first = iss.tellg() - 1l;
        auto last = line.find('"', first);

	if (last != std::string::npos)
	{
	  token += line.substr(first, last - first + 1);
	  iss.seekg(last + 1);
	}
      }
      
      if (!token.empty()) tokens.push_back(token);
    }

    //for (auto t : tokens) std::cout << t << "\n";

    return tokens;
  }


  /*******************************************/
  /* Remove all char occurrences from string */
  /*******************************************/
  void strip_char(const char c, std::string &str)
  {
    str.erase(std::remove(str.begin(), str.end(), c), str.end()); 
  }
};
