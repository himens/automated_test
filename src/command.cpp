#include "command.h"

/**********/
/* Constr */
/**********/
Command::Command(const std::string name, const std::vector<std::string> &args_names)
{ 
  _name = name;

  for (auto name : args_names) 
  {
    _args.push_back(name);
  }
}


/***********/
/* Set/get */
/***********/
void Command::set_args_values(const std::vector<std::string> &values)
{
  if (values.size() == _args.size()) 
  {
    for (size_t i = 0; i < values.size(); i++)
    {
      _args[i].set_value( values[i] );
    }
  }
  else 
  {
    throw Error("Command::set_args_values: command '" + _name + "' needs " + std::to_string(_args.size()) + 
	" arguments, " + std::to_string(values.size()) + " provided!");
  }
}

const std::string Command::get_arg_value(const std::string name) const
{
  auto it = std::find_if(_args.begin(), _args.end(), 
      [&] (Variable arg) { return arg.get_name() == name; });

  if (it == _args.end())
  {
    throw Error("Command::get_arg: '" + name + "' not an argument of command '" + _name + "'!");
  }
    
  return it->get_value();
};

void Command::set_name(const std::string name) 
{ 
  if (name.empty()) 
  {
    throw Error("Command::set_name: name is empty!");
  }

  _name = name; 
}
