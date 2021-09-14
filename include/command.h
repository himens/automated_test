#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <fstream>

#include "error.h"
#include "variable.h"

/* Command class: it stores command name + its args */
class Command
{
  public:
    Command(const std::string name, const std::vector<std::string> &args_names);

    /* Run command */
    virtual void run() = 0;
    void run(const std::vector<std::string> &values);

    /* Write command report to file */
    void write_report(std::ofstream &file) const;

    /* Set/get */
    void set_args_values(const std::vector<std::string> &values);
    void set_name(const std::string name);
    void set_report_msg(const std::string msg);
    
    const std::vector<Variable>& get_args() const { return _args; }
    const std::string get_arg_value(const std::string name) const;
    const std::string get_name() const { return _name; }
    const std::string get_report_msg() const { return _report_msg; }

  private:
    std::string _name;
    std::string _report_msg;
    std::vector<Variable> _args;
};
#endif
