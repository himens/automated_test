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

#include "function_store.h"
#include "command.h"

class Test 
{
  public:
    Test () {};
    Test (const std::string filename) { parse_test(filename); };
    ~Test () {};
    
    /* Parse test file */
    void parse_test(const std::string filename);

    /* run test */
    void run() { for (auto fun : _functions) fun(); } 

  private:
    /* get command function */
    std::function<void()> get_cmd_function(const std::string name, const std::vector<std::string> &args);

    std::vector<std::function<void()>> _functions = {};
    FunctionStore _fun_store;
    std::map<std::string, UserCommand> _user_command = {};
};
#endif
