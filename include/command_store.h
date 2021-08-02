#ifndef COMMAND_STORE_H
#define COMMAND_STORE_H

#include "command.h"
#include "utils.h"

/* UserCmd class: define a user-command which is a set of already existing commands 
   plus placeholders which stands for numeric arguments to be passed when called. 
   The user-command can be seen as new function with its arguments.
 */
class UserCmd : public Command
{
  public:
    UserCmd() {}
    UserCmd(const std::string name, 
	    const std::vector<std::string> &pholds, 
	    const std::vector<std::shared_ptr<Command>> &cmds) : Command(name, pholds.size()) 
    { 
      _commands = cmds;

      for (size_t i = 0; i < pholds.size(); i++) 
	if (Placeholder::is_placeholder(pholds[i])) _placeholders.push_back(pholds[i]); 
    }

    void run()
    {
      // replace placeholders in command
      auto replace = [&](const std::vector<std::string> args)
      {
	auto new_args = args;
	for (size_t i = 0; i < _placeholders.size(); i++)
	{
	  auto it = std::find(new_args.begin(), new_args.end(), _placeholders[i].to_string());
	  if (it != new_args.end()) *it = this->get_args().at(i);
	}

	return new_args;
      };

      for (auto cmd : _commands) 
      {
	auto args = cmd->get_args();
	cmd->set_args(replace(args));
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

      if (utils::is_digit(a) && utils::is_digit(b)) passed = std::abs(std::stof(a) - std::stof(b)) < 1e-5;
      else if (!utils::is_digit(a) && !utils::is_digit(b)) passed = a == b;
      else std::cout << "WARNING: checking data of different types!" << std::endl;

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
      int thrust = utils::is_digit(value) ? std::stoi(value) : 0;
      if (!utils::is_digit(value)) std::cout << "WARNING: set_thrust(): value '" << value << "' is not a number! \n";

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
