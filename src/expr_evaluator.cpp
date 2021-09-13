#include "expr_evaluator.h"

/********************/
/* Detail namespace */
/********************/
namespace Detail
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

  // convert to postfix
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

  // evaluate postfix
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
};


/**********/
/* Constr */
/**********/
ExprEvaluator::ExprEvaluator() {};


/*******************************/
/* Evaluate logical expression */
/*******************************/
bool ExprEvaluator::eval(const std::string expr) const
{
  auto postfix = Detail::to_postfix(expr);
  //std::cout << postfix << "\n";
  bool result = Detail::eval_postfix(postfix);

  return result;
}
