#ifndef COMMAND_H
#define COMMAND_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>
#include <functional>

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
struct Command
{
  std::string name;
  std::vector<std::string> args;

  /* Replace placeholders with values */
  std::vector<std::string> replace(const std::vector<Placeholder> &pholders, 
                                   const std::vector<std::string> &values)
  {
    std::vector<std::string> rep_args = args;

    if (values.size() != pholders.size())
    {
      std::cout << "[WARNING] Command::subsitute(): cannot replace placeholders";
      std::cout << " for '" << name << "' command! Num. of placeholders != num. values! \n";
      return args;
    }

    for (size_t i = 0; i < pholders.size(); i++)
    {
      auto it = std::find(rep_args.begin(), rep_args.end(), pholders[i].to_string());
      if (it != rep_args.end()) *it = values[i];
    }

    return rep_args;
  }
};

/* UserCommand class: define a user-command which is a set of already existing commands 
   plus placeholders which stands for numeric arguments to be passed when called. 
   The user-command can be seen as new function with its arguments.
 */
struct UserCommand
{
  std::vector<Command> commands;
  std::vector<Placeholder> placeholders;
};
#endif
