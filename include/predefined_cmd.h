#ifndef PREDEFINED_CMD_H
#define PREDEFINED_CMD_H

#include "command.h"
#include "utils.h"
#include "variable.h"
#include "logical_expr.h"

/* Check command */
class CheckCmd : public Command
{
  public:
    CheckCmd() : Command("check", 1) {}

    /* Run command */
    void run()
    {
      auto expr = this->get_args().at(0);
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
    void write_report(std::ofstream &file) const
    {
      if (!file.is_open())
      {
        throw Error("cannot open report file!");
      }

      file << "1. Check: " << _expr.get_expr() << " " << (_expr.eval() ? "**PASSED**" : "**FAILED**") << "\n";
    }

  private: 
    LogicalExpr _expr;
};

/* Set thrust command */
class SetThrustCmd : public Command
{
  public:
    SetThrustCmd() : Command("set_thrust", 1) {}

    /* Run command */
    void run()
    {
      auto value = this->get_args().at(0);
      if (!Utils::is_digit(value)) 
      {
	throw Error("SetThrustCmd: value '" + value + "' is not a number!");
      }

      _thrust = std::stoi(value);
      std::cout << "SetThrustCmd: thrust set to " << _thrust << "\n"; 
    }

    /* Write command report to file */
    void write_report(std::ofstream &file) const 
    {
      if (!file.is_open())
      {
        throw Error("cannot open report file!");
      }
      
      file << "1. Thrust set to: " << _thrust << "\n";
    }

  private:
    int _thrust = 0; 
};

/* Insert PDS command */
class InsertPdsCmd : public Command
{
  public:
    InsertPdsCmd() : Command("insert_pds", 0) {}

    /* Run command */
    void run() 
    {
      _pds_inserted = true;
      std::cout << "InsertPdsCmd: pds " << (_pds_inserted ? "inserted" : "not inserted") << "\n"; 
    }
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const 
    {
      if (!file.is_open())
      {
        throw Error("cannot open report file!");
      }
      
      file << "1. Pds status: " << (_pds_inserted ? "inserted" : "not inserted") << "\n";
    }

  private:
    bool _pds_inserted = false; 
};
#endif
