#include "utils.h"

  namespace Utils
  {
    /********************************/
    /* Tell if string is alphabetic */
    /********************************/
    bool is_alphabetic(const std::string str)
    {
      return !str.empty() ? std::all_of(str.begin(), str.end(), ::isalpha) : false;
    };


    /***************************/
    /* Tell if string is digit */
    /***************************/
    bool is_digit(const std::string str)
    {
      return !str.empty() ? (str.find_first_not_of("0123456789.") == std::string::npos) : false;
    }; 


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
	while (std::getline(iss, token, delim)) tokens.push_back(token);
	tokens.push_back(line.substr(first, last));
      }

      // tokenize whole line
      else
      {
      	std::istringstream iss(line);
	while (std::getline(iss, token, delim)) tokens.push_back(token);
      }

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
  

  /***********************************/
  /* Evaluate conditional expression */
  /***********************************/
  bool eval_conditional_expr(const std::string expr)
  {
    // tell if str is relational expr
    auto is_relational = [] (const std::string str)
    {
      if (str == ">" || str == "<" || str == "!=" || 
	  str == ">="  || str == "<=" || str == "==") return true;
      else return false;
    };

    // tell if str is logical expr
    auto is_logical = [] (const std::string str)
    {
      if (str == "&&" || str == "||" || str == "!") return true;
      else return false;
    };

    // convert to postfix
    auto to_postfix = [&] (const std::string infix)
    {
      std::stack<std::string> s;
      std::string postfix;

      for (size_t i = 0; i < infix.length(); i++)
      {
	std::string c = { infix[i] };

	//  detect operand
	if (Utils::is_digit(c) || Utils::is_alphabetic(c))
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
    };

    // evaluate postfix
    auto eval_postfix = [&] (const std::string postfix)
    {
      // perform logical op.
      auto logical_op = [] (const std::string op, const bool op1, const bool op2)
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

      // perform relational op.
      auto relational_op = [] (const std::string op, const float op1, const float op2)
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

      bool result = false;
      std::stack<float> s;

      for (size_t i = 0; i < postfix.length(); i++) 
      {
        std::string c = { postfix[i] };

	// detect operator
	if (c.find_first_of("><&|!=") != std::string::npos)
	{
	  auto op = c; 
	  if (i + 1 < postfix.length()) // catch 2-char operator
	  {
	    std::string c2 = { postfix[i+1] };
	    if (c2.find_first_of("><&|!=") != std::string::npos) op += c2;
	  }

	  // if operator, pop two operands and perform op.
	  if (is_relational(op)) 
	  {
	    auto operand1 = s.top(); 
	    s.pop();

	    auto operand2 = s.top(); 
	    s.pop();

	    auto res = relational_op(op, operand2, operand1);
	    std::cout << operand2 << " " << op << " " << operand1 << " " << res << "\n";
	    s.push(res);
	  }

	  else if (is_logical(op))
	  {
	    auto operand1 = s.top(); 
	    s.pop();

	    auto operand2 = s.top(); 
	    s.pop();

	    auto res = logical_op(op, operand2, operand1);
	    std::cout << ">>>>>> " << operand2 << " " << op << " " << operand1 << " " << res << "\n";
	    s.push(res);
	  }
	}

	// push numeric operand on stack
	else if (Utils::is_digit(c))
        {
	  std::string operand; 

          while (i < postfix.length()) 
          {
	    c = { postfix[i] };
            if (!Utils::is_digit(c)) break;

	    operand += c;
            i++;
          }
          i--;

          s.push(std::stof(operand));
        }
	
	// push alphabetic operand on stack
	else if (Utils::is_alphabetic(c))
	{
	  std::string operand; 

          while (i < postfix.length()) 
          {
	    c = { postfix[i] };
            if (!Utils::is_alphabetic(c)) break;

	    operand += c;
            i++;
          }
          i--;

          s.push(3); // rimpiazzare varibile con valore!!!
	}
      }

      return result;
    };

    auto postfix = to_postfix(expr);
    std::cout << "postfix: " << postfix << "\n";
    bool result = eval_postfix(postfix);

    return result;
  }

  

};
