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

    /* Get command */
    std::shared_ptr<Command> get_command(const std::string name);

  private:
    std::vector<Step> _steps = {};
    std::vector<UserCmd> _user_commands = {};
    std::vector<Variable> _variables = {};
};
#endif
