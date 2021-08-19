#ifndef TEST_H
#define TEST_H

#include "utils.h"
#include "commands.h"
#include "variable.h"
#include "step.h"

class Test 
{
  public:
    Test () {};
    Test (const std::string filename) { parse_test(filename); };
    
    /* Parse test file */
    void parse_test(const std::string filename);

    /* Run test */
    void run();

    /* Make command object */
    std::shared_ptr<Command> make_command(const std::string name)
    {
      std::shared_ptr<Command> cmd = nullptr;

      auto it = std::find_if(_user_commands.begin(), _user_commands.end(), 
	  [&] (UserCmd cmd) { return name == cmd.get_name(); });

      if (name == "set_thrust") cmd = std::make_shared<SetThrustCmd>();
      else if (name == "insert_pds") cmd = std::make_shared<InsertPdsCmd>();
      else if (name == "check") cmd = std::make_shared<CheckCmd>();
      else if (it != _user_commands.end()) 
      {
	auto usr_cmd = std::make_shared<UserCmd>(*it);

	// allocate new commands (otherwise the same of *it will be used...)
	std::vector<std::shared_ptr<Command>> commands = {};
	for (auto cmd : usr_cmd->get_commands())
	{
	  auto c = make_command( cmd->get_name() );
	  c->set_args( cmd->get_args() );
	  commands.push_back(c);
	}

	usr_cmd->set_commands(commands);
	cmd = usr_cmd;
      }
      else 
      {
	throw Error("Unknown command '" + name + "'!");
      }

      return cmd;
    }

  private:
    std::vector<Step> _steps = {};
    std::vector<UserCmd> _user_commands = {};
    std::vector<Variable> _variables = {};
};
#endif
