#include "predefined_cmd.h"

/*****************/
/* Check command */
/*****************/
/* Replace variables in expr */
void CheckCmd::replace_variables(std::string &expr) const
{
  auto begin = expr.find("$");
  auto end = expr.find(" ");

  while (begin != std::string::npos && end != std::string::npos) 
  {
    auto var = expr.substr(begin, end - begin);
    expr.replace(begin, var.length(), "1"); // replace with mem. area data
    //std::cout << "var: " << var << "\n";

    begin = expr.find("$");
    end = expr.find(" ", begin);
  }
}

/* Run command */
void CheckCmd::run()
{
  auto expr = get_arg_value("$expr");
  replace_variables(expr);

  std::cout << "Check: " << get_arg_value("$expr") << " " << (Utils::eval_logical_expr(expr) ? "PASSED" : "FAILED") << "\n";
}

/* Write command report to file */
void CheckCmd::write_report(std::ofstream &file) const
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  auto expr = get_arg_value("$expr");
  replace_variables(expr);

  file << "1. Check: " << get_arg_value("$expr") << " " << (Utils::eval_logical_expr(expr) ? "**PASSED**" : "**FAILED**") << "\n";
}


/**********************/
/* Set thrust command */
/**********************/
/* Run command */
void SetThrustCmd::run()
{
  auto thrust = get_arg_value("$thrust");
  if (!Utils::is_digit(thrust)) 
  {
    throw Error("SetThrustCmd: value '" + thrust + "' is not a number!");
  }

  std::cout << "SetThrustCmd: thrust set to " << thrust << "\n"; 
}

/* Write command report to file */
void SetThrustCmd::write_report(std::ofstream &file) const 
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  file << "1. Thrust set to: " << get_arg_value("$thrust")  << "\n";
}


/**********************/
/* Insert PDS command */
/**********************/
/* Run command */
void InsertPdsCmd::run() 
{
  std::cout << "InsertPdsCmd: pds inserted \n"; 
}

/* Write command report to file */
void InsertPdsCmd::write_report(std::ofstream &file) const 
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  file << "1. Pds status: inserted \n";
}
