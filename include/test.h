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

    const std::string get_name() const { return _name; }
    const std::map<std::string, Step>& get_step_map() const { return _step_map; }
    const std::map<std::string, UserCmd>& get_user_command_map() { return _user_command_map; } 

  private:
    std::string _name;
    std::map<std::string, Step> _step_map = {};
    std::map<std::string, UserCmd> _user_command_map = {};
};
#endif
