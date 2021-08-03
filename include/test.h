#ifndef TEST_H
#define TEST_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

#include "utils.h"
#include "command_store.h"

class Test 
{
  public:
    Test () {};
    Test (const std::string filename) { parse_test(filename); };
    ~Test () {};
    
    /* Parse test file */
    void parse_test(const std::string filename);

    /* Run test */
    void run();

    /* Get command */
    std::shared_ptr<Command> get_command(const std::string name)
    {
      std::shared_ptr<Command> cmd = nullptr;

      if (name == "\\set_thrust") cmd = std::make_shared<SetThrustCmd>();
      else if (name == "\\insert_pds") cmd = std::make_shared<InsertPdsCmd>();
      else if (name == "\\check") cmd = std::make_shared<CheckCmd>();
      else if (_user_command_map.count(name) > 0) cmd = std::make_shared<UserCmd>(_user_command_map[name]);
      else 
      {
	std::cout << "ERROR: Unknown command '" << name << "'! \n";
	return nullptr;
      }

      return cmd;
    }

  private:
    /* Print banner */
    void print_banner(const std::string, size_t length = 0);

    bool _file_parsed = false;
    std::vector<std::shared_ptr<Command>> _commands = {};
    std::map<std::string, UserCmd> _user_command_map = {};
};
#endif
