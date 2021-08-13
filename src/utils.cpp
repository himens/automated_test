#include "utils.h"

  namespace Utils
  {
    /********************************/
    /* Tell if string is alphabetic */
    /********************************/
    bool is_alphabetic(const std::string str)
    {
      return !str.empty() && std::all_of(str.begin(), str.end(), ::isalpha);
    };


    /***************************/
    /* Tell if string is digit */
    /***************************/
    bool is_digit(const std::string str)
    {
      return !str.empty() && (str.find_first_not_of("0123456789.") == std::string::npos);
    }; 


    /*****************/
    /* Tokenize line */
    /*****************/
    std::vector<std::string> tokens(std::string line, const char delim) 
    {
      std::vector<std::string> tokens;
      std::string token;
      std::istringstream iss(line);

      while (std::getline(iss, token, delim)) tokens.push_back(token);

    return tokens;
  };

  /*******************************************/
  /* Remove all char occurrences from string */
  /*******************************************/
  void strip_char(const char c, std::string &str)
  {
    str.erase(std::remove(str.begin(), str.end(), c), str.end()); 
  };

  /***********************************/
  /* Get all tokens except first one */
  /***********************************/
  std::vector<std::string> remove_first_token(const std::vector<std::string> &tokens)
  {
    auto args = tokens;
    if (tokens.size() > 1) args.erase(args.begin());
    else args = {};

    return args;
  };
  

  /********************************************/
  /* Convert expression from infix to postfix */
  /********************************************/
  std::string to_postfix(const std::string infix)
  {
    auto is_relational = [] (const std::string str)
    {
      if (str == ">" || str == "<" || str == "!=" || 
	  str == ">="  || str == "<=" || str == "==") return true;
      else return false;
    };

    auto is_logical = [] (const std::string str)
    {
      if (str == "&&" || str == "||" || str == "!") return true;
      else return false;
    };

    std::stack<std::string> s;
    std::string postfix;

    for (size_t i = 0; i < infix.length(); i++)
    {
      std::string c = { infix[i] };

      //  detect operand
      if (Utils::is_digit(c) || Utils::is_alphabetic(c) || c == "_")
      {
	postfix += c;
      }

      // detect begin of parenthesis block 
      else if (c == "(")
      {
	s.push(c);
      }

      // detect end of parenthesis block 
      else if (c == ")")
      {
	while(s.top() != "(" && !s.empty())
	{
	  auto temp = s.top();
	  postfix += temp;
	  s.pop();
	}

	if (s.top() == "(") s.pop();
      }

      // detect operator
      else if (c.find_first_of("><&|!=") != std::string::npos)
      {
	auto op = c; // catch 2-char operator
	if (i + 1 < infix.length())
	{
	  std::string c2 = { infix[i+1] };
	  if (c2.find_first_of("><&|!=") != std::string::npos) op += c2;
	}

	if (is_logical(op) || is_relational(op)) s.push(op);
      }
    } 

    while(!s.empty())
    {
      postfix += s.top();
      s.pop();
    }

    return postfix;
  }

};
