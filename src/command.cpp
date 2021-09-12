#include "command.h"

/**********/
/* Constr */
/**********/
Command::Command(const size_t num_args) 
{ 
  _args.resize(num_args); 

  for (size_t i = 0; i < _args.size(); i++)
  {
    _args[i] = "$arg_" + std::to_string(i);
  }
}

Command::Command(const std::string name, const size_t num_args) : Command(num_args) 
{ 
  set_name(name);
}

Command::Command(const std::string name, const std::vector<Variable> &args) : Command(name, args.size())
{ 
  set_args(args);
}


/***********/
/* Set/get */
/***********/
void Command::set_args(const std::vector<Variable> &args)
{
  if (args.size() == _args.size()) 
  {
      _args = args;
  }
  else 
  {
    throw Error("Command::set_args: command '" + _name + "' needs " + std::to_string(_args.size()) + 
	" arguments, " + std::to_string(args.size()) + " provided!");
  }
}

void Command::set_args_values(const std::vector<std::string> &values)
{
  if (values.size() == _args.size()) 
  {
    for (size_t i = 0; i < values.size(); i++)
    {
      _args[i].set_value(values[i]);
    }
  }
  else 
  {
    throw Error("Command::set_args: command '" + _name + "' needs " + std::to_string(_args.size()) + 
	" arguments, " + std::to_string(values.size()) + " provided!");
  }
}

void Command::set_name(const std::string name) 
{ 
  if (name.empty()) 
  {
    throw Error("Command::set_name: name is empty!");
  }

  _name = name; 
}
