#ifndef COMMANDS_H
#define COMMANDS_H

#include "command.h"
#include "utils.h"
#include "variable.h"
#include "logical_expr.h"

/* UserCmd class: define a user-command which is a set of already existing commands 
   plus placeholders which stands for numeric arguments to be passed when called. 
   The user-command can be seen as new function with its arguments.
 */
class UserCmd : public Command
{
  public:
    UserCmd() {}
    UserCmd(const std::string name, 
	    const std::vector<std::string> &placeholders, 
	    const std::vector<std::shared_ptr<Command>> &commands) : Command(name, placeholders.size()) 
    { 
      set_placeholders(placeholders);
      set_commands(commands);
    }

    void run()
    {
      // replace placeholders 
      auto replace_placeholder_with_val = [&](const std::vector<std::string> &args)
      {
	auto args_r = args;

	for (size_t i = 0; i < _placeholders.size(); i++)
	{
	  for (auto &arg : args_r) 
	  {
	    if (_placeholders[i] == arg) arg = this->get_args().at(i);
	  }
	}

	return args_r;
      };

      for (auto cmd : _commands) 
      {
	auto args = cmd->get_args(); 
	cmd->set_args( replace_placeholder_with_val(args) );
	cmd->run();
	cmd->set_args(args);
      }
    }
    
    /* Set/get */
    void set_commands(const std::vector<std::shared_ptr<Command>> &commands) { _commands = commands; }
    
    void set_placeholders(const std::vector<std::string> &placeholders) 
    { 
      auto it = std::find_if(placeholders.begin(), placeholders.end(), [] (std::string s) { return s.front() != '_'; });
      if (it != placeholders.end()) 
      {
	throw Error("UserCmd::set_placeholders: '" + *it + "' not a placeholder! It should begin with '_'!");
      }

      _placeholders = placeholders;
    }
    
    const std::vector<std::shared_ptr<Command>> get_commands() const { return _commands; }
    const std::vector<std::string> get_placeholders() const { return _placeholders; }

  private:
    std::vector<std::shared_ptr<Command>> _commands;
    std::vector<std::string> _placeholders;
};

/* Check command */
class CheckCmd : public Command
{
  public:
    CheckCmd() : Command("check", 1) {}

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

      LogicalExpr logical_expr{expr, variables};
      bool passed = logical_expr.eval();

      std::cout << "Check: " << expr << " " << (passed ? "PASSED" : "FAILED") << "\n";
    }
};

/* Set thrust command */
class SetThrustCmd : public Command
{
  public:
    SetThrustCmd() : Command("set_thrust", 1) {}

    void run()
    {
      auto value = this->get_args().at(0);
      int thrust = Utils::is_digit(value) ? std::stoi(value) : 0;
      if (!Utils::is_digit(value)) 
      {
	throw Error("SetThrustCmd: value '" + value + "' is not a number!");
      }

      std::cout << "SetThrustCmd: thrust set to " << thrust << "\n"; 
    }
};

/* Insert PDS command */
class InsertPdsCmd : public Command
{
  public:
    InsertPdsCmd() : Command("insert_pds", 0) {}

    void run() { std::cout << "InsertPdsCmd: pds inserted! \n"; }
};
#endif
