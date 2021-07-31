#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>

/* Parameter class: a string beginning with "_" */
class Parameter
{
  public:
    Parameter(const std::string par) { if (is_parameter(par)) _par = par; }

    friend std::ostream& operator<< (std::ostream& os, const Parameter &par)
    {
      os << par.to_string();
      return os;
    }

    /* Convert to std::string */
    std::string to_string() const { return _par; }

    /* Tell if a string is a parameter */
    static bool is_parameter(const std::string par) { return par.front() == '_'; } 

  private:
    std::string _par;
};

/* Command class: store command name + its args */
struct Command
{
  std::string name;
  std::vector<std::string> args;

  /* Replace a parameter in args with a value */
  void replace_parameter(const Parameter par, const std::string value)
  {
    auto it = std::find(args.begin(), args.end(), par.to_string());
    if (it == args.end()) return;
    
   *it = value;
  }
};

struct UserCommand
{
  std::string name;
  std::vector<Command> commands;
  std::vector<Parameter> parameters;

  /* Replace parameters with values */
  void replace_parameters(const std::vector<std::string> &values)
  {
    if (values.size() != parameters.size())
    {
      std::cout << "[WARNING] UserCommand::replace_parameters(): cannot replace parameters";
      std::cout << " for '" << name << "'! Num. of parameters != num. values! \n";
      return;
    }

    for (size_t i = 0; i < parameters.size(); i++)
      for (auto &cmd : commands) cmd.replace_parameter(parameters[i], values[i]);
  }
};
#endif



