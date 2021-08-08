#ifndef COMMAND_STORE_H
#define COMMAND_STORE_H

#include "command.h"
#include "utils.h"
#include "tagged_string.h"

/* UserCmd class: define a user-command which is a set of already existing commands 
   plus placeholders which stands for numeric arguments to be passed when called. 
   The user-command can be seen as new function with its arguments.
 */
class UserCmd : public Command
{
  public:
    UserCmd() {}
    UserCmd(const std::string name, 
	    const std::vector<Placeholder> &placeholders, 
	    const std::vector<std::shared_ptr<Command>> &cmds) : Command(name, placeholders.size()) 
    { 
      _commands = cmds;
      _placeholders = placeholders;
    }

    void run()
    {
      // replace placeholders 
      auto replace_placeholders = [&](std::vector<std::string> args)
      {
	for (size_t i = 0; i < _placeholders.size(); i++)
	  for (auto &arg : args) 
	    if (_placeholders[i].to_string() == arg) arg = this->get_args().at(i);

	return args;
      };

      for (auto cmd : _commands) 
      {
	auto args = cmd->get_args();
	cmd->set_args( replace_placeholders(args) );
	cmd->run();
	cmd->set_args(args);
      }
    }
    
  private:
    std::vector<std::shared_ptr<Command>> _commands;
    std::vector<Placeholder> _placeholders;
};

/* Check command */
class CheckCmd : public Command
{
  public:
    CheckCmd() : Command("check", 2) {}

    void run()
    {
      bool passed = false;
      auto a = this->get_args().at(0);
      auto b = this->get_args().at(1);

      if (Utils::is_digit(a) && Utils::is_digit(b)) passed = std::abs(std::stof(a) - std::stof(b)) < 1e-5;
      else if (!Utils::is_digit(a) && !Utils::is_digit(b)) passed = a == b;
      else 
      {
	throw Error("CheckCmd: checking data of different types!");
      }

      std::cout << "check(): " << (passed ? "PASSED" : "FAILED") << std::endl;
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

      std::cout << "set_thrust(): thrust set to " << thrust << "\n"; 
    }
};

/* Insert PDS command */
class InsertPdsCmd : public Command
{
  public:
    InsertPdsCmd() : Command("insert_pds", 0) {}

    void run() { std::cout << "insert_pds(): pds inserted! \n"; }
};
#endif
