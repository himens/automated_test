#ifndef FUNCTION_STORE_H
#define FUNCTION_STORE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <memory>
#include <algorithm>
#include <functional>

class FunctionStore
{
  public:
    FunctionStore() {};
    ~FunctionStore() {};
 
    /* insert_pds */
    void insert_pds();

    /* set_thrust */
    void set_thrust(const std::string value);

    /* check */
    bool check(const std::string a, const std::string b);
   
  private:  
};

#endif
