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
    std::map<std::string, std::function<void()>> _user_cmd_functions = {};

    //////////////////////////////////////////////////////////////////
    // FUNCTIONS 
    //////////////////////////////////////////////////////////////////
    /* insert_pds */
    void insert_pds() { std::cout << "insert_pds! \n"; }

    /* set_thrust */
    void set_thrust(const int value) { std::cout << "set_thrust to " << value << "\n"; }

    /* check */
    bool check(const std::string a, const std::string b);
    ///////////////////////////////////////////////////////////////////
};
#endif
