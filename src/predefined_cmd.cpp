#include "predefined_cmd.h"

/*****************/
/* Check command */
/*****************/
CheckCmd::CheckCmd() : Command("check", 1) {}

/* Run command */
void CheckCmd::run()
{
  auto expr = get_args().at(0).get_value();
  auto begin = expr.find("$");
  auto end = expr.find(" ");
  std::vector<Variable> variables;

  while (begin != std::string::npos && end != std::string::npos) 
  {
    auto var = expr.substr(begin, end - begin);
    variables.push_back({var, "1"});

    begin = expr.find("$", end + 1);
    end = expr.find(" ", begin);
  }

  _expr = {expr, variables};

  std::cout << "Check: " << expr << " " << (_expr.eval() ? "PASSED" : "FAILED") << "\n";
}

/* Write command report to file */
void CheckCmd::write_report(std::ofstream &file) const
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  file << "1. Check: " << _expr.get_expr() << " " << (_expr.eval() ? "**PASSED**" : "**FAILED**") << "\n";
}


/**********************/
/* Set thrust command */
/**********************/
SetThrustCmd::SetThrustCmd() : Command("set_thrust", 1) {}

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
InsertPdsCmd::InsertPdsCmd() : Command("insert_pds", 0) {}

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
