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
  auto expr = get_args().at(0).get_value();
  replace_variables(expr);

  std::cout << "Check: " << get_args().at(0).get_value() << " " << (Utils::eval_logical_expr(expr) ? "PASSED" : "FAILED") << "\n";
}

/* Write command report to file */
void CheckCmd::write_report(std::ofstream &file) const
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  auto expr = get_args().at(0).get_value();
  replace_variables(expr);

  file << "1. Check: " << get_args().at(0).get_value() << " " << (Utils::eval_logical_expr(expr) ? "**PASSED**" : "**FAILED**") << "\n";
}


/**********************/
/* Set thrust command */
/**********************/
/* Run command */
void SetThrustCmd::run()
{
  auto value = get_args().at(0).get_value();
  if (!Utils::is_digit(value)) 
  {
    throw Error("SetThrustCmd: value '" + value + "' is not a number!");
  }

  _thrust = std::stoi(value);
  std::cout << "SetThrustCmd: thrust set to " << _thrust << "\n"; 
}

/* Write command report to file */
void SetThrustCmd::write_report(std::ofstream &file) const 
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  file << "1. Thrust set to: " << _thrust << "\n";
}


/**********************/
/* Insert PDS command */
/**********************/
/* Run command */
void InsertPdsCmd::run() 
{
  _pds_inserted = true;
  std::cout << "InsertPdsCmd: pds " << (_pds_inserted ? "inserted" : "not inserted") << "\n"; 
}

/* Write command report to file */
void InsertPdsCmd::write_report(std::ofstream &file) const 
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  file << "1. Pds status: " << (_pds_inserted ? "inserted" : "not inserted") << "\n";
}
