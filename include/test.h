#ifndef TEST_H
#define TEST_H

#include "utils.h"
#include "commands.h"
#include "variable.h"

class Test 
{
  public:
    Test () {};
    Test (const std::string filename) { parse_test(filename); };
    
    /* Parse test file */
    void parse_test(const std::string filename);

    /* Run test */
    void run();

    /* Get command */
    std::shared_ptr<Command> get_command(std::string name)
    {
      std::shared_ptr<Command> cmd = nullptr;

      auto it = std::find_if(_usr_commands.begin(), _usr_commands.end(), 
	  [&] (UserCmd cmd) { return name == cmd.get_name(); });

      if (name == "set_thrust") cmd = std::make_shared<SetThrustCmd>();
      else if (name == "insert_pds") cmd = std::make_shared<InsertPdsCmd>();
      else if (name == "check") cmd = std::make_shared<CheckCmd>();
      else if (it != _usr_commands.end()) cmd = std::make_shared<UserCmd>(*it);
      else 
      {
	throw Error("Unknown command '" + name + "'!");
      }

      return cmd;
    }

  private:
    /* Print banner */
    void print_banner(const std::string, size_t length = 0);

    std::vector<std::shared_ptr<Command>> _commands = {};
    std::vector<UserCmd> _usr_commands = {};
    std::vector<Variable> _variables = {};
    std::map<std::string, std::string> _alias_to_tgt_map = {};
};
#endif
