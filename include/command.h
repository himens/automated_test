#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "error.h"
#include "variable.h"

/* Command class: it stores command name + its args */
class Command
{
  public:
    Command(const size_t num_rgs);
    Command(const std::string name, const size_t num_args);
    Command(const std::string name, const std::vector<Variable> &args);

    /* Run command */
    virtual void run() = 0;

    /* Write command report to file */
    virtual void write_report(std::ofstream &file) const = 0;

    /* Set/get */
    void set_args(const std::vector<Variable> &args);
    void set_args_values(const std::vector<std::string> &values);
    void set_name(const std::string name);
    
    const std::vector<Variable>& get_args() const { return _args; }
    const std::string get_name() const { return _name; }

  private:
    std::string _name;
    std::vector<Variable> _args;
};
#endif
