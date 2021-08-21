#ifndef VARIABLE_H
#define VARIABLE_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "error.h"

/* Variable class: it stores variable name + its value */
class Variable
{
  public:
    Variable();
    Variable(const std::string name);
    Variable(const std::string name, const std::string value);

    /* Get/set */
    void set_value(const std::string value);
    void set_name(const std::string name); 
    
    const std::string get_value() const { return _value; }
    const std::string get_name() const { return _name; }

  private:
    std::string _name;
    std::string _value;
};
#endif
