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
    Variable() {}
    Variable(const std::string name) { set_name(name); }
    Variable(const std::string name, const std::string value) : Variable(name) { set_value(value); }

    Variable& operator= (const Variable &var) 
    { 
     this->set_name(var.get_name());
     this->set_value(var.get_value());
     return *this; 
    }
    bool operator== (const Variable &var) const { return this->get_name() == var.get_name(); }

    /* Get/set */
    void set_name(const std::string name) 
    { 
      if (name.front() != '$')
      {
	throw Error("Variable::set_name: illegal variable name '" + name + "'! Missing '$'!");
      }

      if (name == "$")
      {
	throw Error("Variable::set_name: illegal variable name '" + name + "'!");
      }

      _name = name; 
    }
    void set_value(const std::string value) { _value = value; }

    const std::string get_name() const { return _name; }
    const std::string get_value() const { return _value; }

  private:
    std::string _name;
    std::string _value;
};
#endif
