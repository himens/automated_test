#ifndef TEST_H
#define TEST_H

#include "user_command.h"
#include "step.h"
#include "predefined_cmd.h"

class Test 
{
  public:
    Test();
    Test(const std::string name);
    
    /* Run test */
    void run();

    /* Read test file */
    void read_test_file(const std::string filename);

    /* Make command object */
    std::shared_ptr<Command> make_command(const std::string name);

    /* Set/get */
    void add_user_command(const UserCmd &cmd);
    void add_step(const Step &step);

    void set_name(const std::string name); 
    void set_steps(const std::vector<Step> &steps); 
    void set_user_commands(const std::vector<UserCmd> commands);

    const std::string get_name() const { return _name; }
    const std::vector<Step>& get_steps() const { return _steps; }
    const std::vector<UserCmd>& get_user_commands() { return _user_commands; } 

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

  private:
    std::string _name;
    std::vector<Step> _steps = {};
    std::vector<UserCmd> _user_commands = {};
    std::vector<Variable> _read_variables = {};
};
#endif
