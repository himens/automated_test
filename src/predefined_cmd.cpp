#include "predefined_cmd.h"

/*****************/
/* Check command */
/*****************/
/* Run command */
void CheckCmd::run()
{
  auto expr = get_arg_value("$expr");
  auto begin = expr.find("$");
  auto end = expr.find(" ", begin);

   // replace $AREA::FIELD variable with mem. area data
  while (begin != std::string::npos && end != std::string::npos) 
  {
    auto var = expr.substr(begin, end - begin);
    expr.replace(begin, var.length(), "1"); // replace with mem. area data

    begin = expr.find("$");
    end = expr.find(" ", begin);
  }

  set_report_msg("1. Check: " + get_arg_value("$expr") + " " + (Utils::eval_logical_expr(expr) ? "**PASSED**" : "**FAILED**"));

  std::cout << "Check: " << get_arg_value("$expr") << " " << (Utils::eval_logical_expr(expr) ? "PASSED" : "FAILED") << "\n";
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

  set_report_msg("1. Thrust set to: " + get_arg_value("$thrust"));

  std::cout << "SetThrustCmd: thrust set to " << thrust << "\n"; 
}


/**********************/
/* Insert PDS command */
/**********************/
/* Run command */
void InsertPdsCmd::run() 
{
  set_report_msg("1. Pds status: inserted");

  std::cout << "InsertPdsCmd: pds inserted \n"; 
}

