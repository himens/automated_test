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
    Command();
    Command(const size_t n);
    Command(const std::string name, const size_t n);

    /* Run command */
    virtual void run() = 0;

    /* Write command report to file */
    virtual void write_report(std::ofstream &file) const = 0;

    /* Set/get */
    void set_args(const std::vector<std::string> &args);
    void set_name(const std::string name);
    
    const std::vector<std::string>& get_args() const { return _args; }
    const std::string get_name() const { return _name; }

  private:
    std::string _name;
    std::vector<std::string> _args;
};
#endif
