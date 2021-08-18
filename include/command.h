#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "error.h"

/* Command class: it stores command name + its args */
class Command
{
  public:
    Command() {};
    Command(const size_t n) { _args.resize(n); }
    Command(const std::string name, const size_t n) : Command(n) { set_name(name); }

    /* Run command*/
    virtual void run() = 0;

    /* Set/get */
    void set_args(const std::vector<std::string> &args)
    {
      if (args.size() == _args.size()) _args = args;
      else 
      {
	throw Error("Command::set_args: command '" + _name + "' needs " + std::to_string(_args.size()) + 
	    	   " arguments, " + std::to_string(args.size()) + " provided!");
      }
    }

    void set_name(const std::string name) 
    { 
      if (name.empty()) 
      {
	throw Error("Command::set_name: name is empty!");
      }

      _name = name; 
    }
    
    const std::vector<std::string>& get_args() const { return _args; }
    const std::string get_name() const { return _name; }

  private:
    std::string _name;
    std::vector<std::string> _args;
};
#endif
