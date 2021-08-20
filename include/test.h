#ifndef TEST_H
#define TEST_H

#include "utils.h"
#include "commands.h"
#include "variable.h"
#include "step.h"

class Test 
{
  public:
    Test() {};
    
    /* Run test */
    void run(const std::string filename);

  private:
    /* Make predefined command object */
    std::shared_ptr<Command> make_predefined_cmd(const std::string name)
    {
      std::shared_ptr<Command> cmd = nullptr;

      if (name == "set_thrust") cmd = std::make_shared<SetThrustCmd>();
      else if (name == "insert_pds") cmd = std::make_shared<InsertPdsCmd>();
      else if (name == "check") cmd = std::make_shared<CheckCmd>();

      return cmd;
    }

    /* Make command object */
    std::shared_ptr<Command> make_command(const std::string name);

    /* Parse test file */
    void parse_test_file(const std::string filename);

  private:
    std::vector<Step> _steps = {};
    std::vector<UserCmd> _user_commands = {};
    std::vector<Variable> _variables = {};
};
#endif
