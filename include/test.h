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

    /* run test */
    void run() { for (auto cmd : _commands) cmd->run(); } 

  private:
    /* get command */
    Command* get_command(const std::string name, const std::vector<std::string> &args);

    std::vector<std::function<void()>> _functions = {};
    std::vector<Command*> _commands = {};
    std::map<std::string, UserCmd> _user_command_map = {};
};
#endif
