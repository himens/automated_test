#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "error.h"

/* Placeholder class: it's a std::string starting with "_" */
class Placeholder
{
  public:
    Placeholder(const std::string str) { if (is_placeholder(str)) _pholder = str; }

    friend std::ostream& operator<< (std::ostream& os, const Placeholder &pholder)
    {
      os << pholder.to_string();
      return os;
    }

    /* Convert to std::string */
    std::string to_string() const { return _pholder; }

    /* Tell if a string is a placeholder */
    static bool is_placeholder(const std::string str) { return str.front() == '_'; } 

  private:
    std::string _pholder;
};

/* Command class: it stores command name + its args */
class Command
{
  public:
    Command() {};
    Command(const std::string name, const size_t n) 
    { 
      _args.resize(n);
      _name = name; 
    } 

    /* Run command*/
    virtual void run() = 0;

    /* Set/get */
    void set_args(const std::vector<std::string> &args)
    {
      if (args.size() == _args.size()) _args = args;
      else 
      {
	throw Error("Command '" + _name + "' needs " + std::to_string(_args.size()) + 
	    	    " arguments, " + std::to_string(args.size()) + " provided!");
      }
    }
    const std::vector<std::string> &get_args() const { return _args; }

    std::string get_name() const { return _name; }

  private:
    std::string _name;
    std::vector<std::string> _args;
};
#endif
