#ifndef COMMANDS_H
#define COMMANDS_H

#include "command.h"
#include "utils.h"
#include "variable.h"
#include "logical_expr.h"

/* UserCmd class: define a user-command which can be seen as new command with its own arguments.
   It's a set of already existing commands plus placeholders which stand for user-command arguments. 
 */
class UserCmd : public Command
{
  public:
    UserCmd() {}
    UserCmd(const size_t n) : Command(n) {}

    UserCmd(const std::string name, 
	    const std::vector<std::string> &placeholders) : Command(name, placeholders.size()) 
    { 
      set_placeholders(placeholders); 
    }

    UserCmd(const std::string name, 
  	    const std::vector<std::string> &placeholders, 
	    const std::vector<std::shared_ptr<Command>> &commands) : UserCmd(name, placeholders) 
    { 
      set_commands(commands); 
    } 

    /* Run command */
    void run()
    {
      // replace placeholders 
      auto replace_placeholder_with_val = [&] (std::vector<std::string> &args)
      {
	for (size_t i = 0; i < _placeholders.size(); i++)
	{
	  for (auto &arg : args) 
	  {
	    if (_placeholders[i] == arg) arg = this->get_args().at(i);
	  }
	}
      };

      for (auto cmd : _commands) 
      {
	auto args = cmd->get_args(); 
	auto replaced_args = cmd->get_args(); 
	replace_placeholder_with_val(replaced_args);

	cmd->set_args(replaced_args);
	cmd->run();
	cmd->set_args(args);
      }
    }
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const
    {
      for (auto cmd : _commands) cmd->write_report(file);
    }

    /* Set/get */
    void add_command(const std::shared_ptr<Command> cmd) 
    {
      if (cmd == nullptr) 
      {
	throw Error("UserCmd::add_command: try to add nullptr to command '" + this->get_name() + "'!");
      }

      _commands.push_back(cmd);
    }

    void set_commands(const std::vector<std::shared_ptr<Command>> &commands) 
    { 
      for (const auto &cmd : commands) add_command(cmd); 
    }

    void set_placeholders(const std::vector<std::string> &placeholders) 
    { 
      auto it = std::find_if(placeholders.begin(), placeholders.end(), 
	  [] (std::string s) { return s.empty() || (!s.empty() && s.front() != '_'); });

      if (it != placeholders.end()) 
      {
	throw Error("UserCmd::set_placeholders: '" + *it + "' not a placeholder! It should begin with '_'!");
      }

      if (placeholders.size() != this->get_args().size())
      {
	throw Error("UserCmd::set_placeholders: command '" + this->get_name() + "' needs " + std::to_string(this->get_args().size()) 
	    + " placeholders, " + std::to_string(placeholders.size()) + " provided!");
      }

      _placeholders = placeholders;
    }
    
    const std::vector<std::shared_ptr<Command>>& get_commands() const { return _commands; }
    const std::vector<std::string>& get_placeholders() const { return _placeholders; }

  private:
    std::vector<std::shared_ptr<Command>> _commands;
    std::vector<std::string> _placeholders;
};

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
