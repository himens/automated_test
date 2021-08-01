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
	    const std::vector<std::string> &args, 
	    const std::vector<Command*> cmds) : Command(name, args.size()) 
    { 
      _commands = cmds;
      set_placeholders(args);
    }
    //~UserCmd() { for (auto cmd : _commands) delete cmd; }

    void run()
    {
      for (auto cmd : _commands) 
      {
	auto args = replace_placeholders(cmd);
	cmd->run(args);
      }
    }
    
    /* Replace placeholders with values */
    std::vector<std::string> replace_placeholders(Command* cmd)
    {
      auto cmd_args = cmd->get_args();

      for (size_t i = 0; i < _placeholders.size(); i++)
      {
	auto it = std::find(cmd_args.begin(), cmd_args.end(), _placeholders[i].to_string());
	if (it != cmd_args.end()) *it = _args[i];
      }

      return cmd_args;
    }

    /* Set placeholders */
    void set_placeholders(const std::vector<std::string> &pholds) 
    {
      for (size_t i = 0; i < pholds.size(); i++) 
      {
	if (Placeholder::is_placeholder(pholds[i])) _placeholders.push_back(pholds[i]); 
      }
    }

  private:
    std::vector<Command*> _commands;
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
      auto a = _args[0];
      auto b = _args[1];

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
      auto value = _args[0];
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
