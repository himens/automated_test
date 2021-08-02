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
    std::shared_ptr<Command> get_command(const std::string name, const std::vector<std::string> &args);

  private:
    /* Print banner */
    void print_banner(const std::string, size_t length = 0);

    bool _file_parsed = false;
    std::vector<std::shared_ptr<Command>> _commands = {};
    std::map<std::string, UserCmd> _user_command_map = {};
};
#endif
