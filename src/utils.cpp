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
  

  /*******************************/
  /* Evaluate logical expression */
  /*******************************/
  bool eval_logical_expr(const std::string expr)
  {
    // utility function: tell if str is relational expr
    auto is_relational_op = [] (const std::string str)
    {
      return (str == ">" || str == "<" || str == "!=" || 
	  str == ">=" || str == "<=" || str == "==");
    };

    // utility function: tell if str is logical expr
    auto is_logical_op = [] (const std::string str)
    {
      return (str == "&&" || str == "||" || str == "!");
    };

    // utility function: tell if str is operator
    auto is_operator = [&] (const std::string str)
    {
      return is_relational_op(str) || is_logical_op(str);
    };

    // utility function: perform logical op.
    auto do_logical_op = [] (const std::string op, const bool op1, const bool op2)
    {
      bool result = false;

      if (op == "&&") result = op1 && op2;
      else if (op == "||") result = op1 || op2;
      else 
      {
	throw Error("unknown logical operation '" + op + "'!");
      }

      return result;
    };

    // utility function: perform relational op.
    auto do_relational_op = [] (const std::string op, const float op1, const float op2)
    {
      bool result = false;

      if (op == ">") result = op1 > op2;
      else if (op == ">=") result = op1 >= op2;
      else if (op == "<") result = op1 < op2;
      else if (op == "<='") result = op1 <= op2;
      else if (op == "==") result = op1 == op2;
      else 
      {
	throw Error("unknown relational operation '" + op + "'!");
      }

      return result;
    };

    // utility function: convert to postfix
    auto to_postfix = [&] (const std::string infix)
    {
      std::stack<std::string> s;
      std::string postfix;

      for (size_t i = 0; i < infix.length(); i++)
      {
	std::string c{infix[i]};

	// catch 2-char operator
	if (!is_operator(c))
	{
	  if ((i+1 < infix.length()) && is_operator(c + infix[i+1])) c += infix[++i];
	}

	// add alphabetic operand
	if (Utils::is_alphabetic(c))
	{
	  std::string var{c};

	  while ((i+1 < infix.length()) && infix[i+1] != ' ') var += infix[++i];

	  postfix += var + " ";
	}

	//  add numeric operand
	else if (Utils::is_digit(c)) 
	{
	  std::string num{c};

	  while ((i+1 < infix.length()) && Utils::is_digit({infix[i+1]})) num += infix[++i];

	  postfix += num + " ";
	}

	// add operator
	else if (is_operator(c)) 
	{
	  s.push(c + " ");
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
	    auto tmp = s.top();
	    postfix += tmp;
	    s.pop();
	  }

	  if (s.top() == "(") s.pop();
	}
      } 

      while (!s.empty())
      {
	postfix += s.top();
	s.pop();
      }

      //std::cout << "postfix: " << postfix << "\n";

      return postfix;
    };

    // utility function: evaluate postfix
    auto eval_postfix = [&] (const std::string postfix)
    {
      std::stack<float> s;
      bool result = false;

      for (size_t i = 0; i < postfix.length(); i++) 
      {
	std::string c{postfix[i]};

	// catch 2-char operator
	if (!is_operator(c))
	{
	  if ((i+1 < postfix.length()) && is_operator(c + postfix[i+1])) c += postfix[++i];
	}

	// if operator, pop two operands and perform op.
	if (is_operator(c)) 
	{
	  auto op2 = s.top(); 
	  s.pop();

	  auto op1 = s.top(); 
	  s.pop();

	  bool res = is_relational_op(c) ? do_relational_op(c, op1, op2) : do_logical_op(c, op1, op2);

	  //std::cout << "operator: " << c << "\n";
	  //std::cout << "result: " << op1 << " " << c << " " << op2 << " " << res << "\n";

	  s.push(res);
	}

	// push numeric operand on stack
	else if (Utils::is_digit(c))
	{
	  std::string op{c}; 

	  while ((i+1 < postfix.length()) && Utils::is_digit({postfix[i+1]})) op += postfix[++i];

	  //std::cout << "numeric op: " << op << "\n";

	  s.push(std::stof(op));
	}
      }

      while (!s.empty())
      {
	result = static_cast<bool>(s.top());
	s.pop();
      }

      return result;
    };

    auto postfix = to_postfix(expr);
    //std::cout << postfix << "\n";
    bool result = eval_postfix(postfix);

    return result;
  }
};
