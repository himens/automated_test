#include "command.h"

/**********/
/* Constr */
/**********/
Command::Command(const size_t n) 
{ 
  _args.resize(n); 
}

Command::Command(const std::string name, const size_t n) : Command(n) 
{ 
  set_name(name);
}


/***********/
/* Set/get */
/***********/
void Command::set_args(const std::vector<std::string> &args)
{
  if (args.size() == _args.size()) _args = args;
  else 
  {
    throw Error("Command::set_args: command '" + _name + "' needs " + std::to_string(_args.size()) + 
	" arguments, " + std::to_string(args.size()) + " provided!");
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
