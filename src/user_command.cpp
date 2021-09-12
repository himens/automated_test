#include "user_command.h"

/**********/
/* Constr */
/**********/
UserCmd::UserCmd(const size_t n) : Command(n) {}

UserCmd::UserCmd(const std::string name, 
	         const std::vector<Variable> &args) : Command(name, args) {}

UserCmd::UserCmd(const std::string name, 
                 const std::vector<Variable> &args, 
                 const std::vector<std::shared_ptr<Command>> &commands) : Command(name, args) 
{ 
  set_commands(commands); 
} 

 
/***************/
/* Run command */
/***************/
void UserCmd::run()
{
  for (auto cmd : _commands) 
  {
    // set cmd args values 
    std::vector<std::string> values; 
    
    for (auto arg : cmd->get_args()) 
    {
      auto it = std::find_if(get_args().begin(), get_args().end(), 
	  [&] (Variable usr_arg) { return usr_arg.get_name() == arg.get_value(); });

      auto value = it != get_args().end() ? it->get_value() : arg.get_value(); 
      values.push_back(value);
    }

    cmd->set_args_values(values);
    cmd->run();
  }
}


/********************************/
/* Write command report to file */
/********************************/
void UserCmd::write_report(std::ofstream &file) const
{
  for (auto cmd : _commands) cmd->write_report(file);
}


/***********/
/* Set/get */
/***********/
void UserCmd::add_command(const std::shared_ptr<Command> cmd) 
{
  if (cmd == nullptr) 
  {
    throw Error("UserCmd::add_command: try to add nullptr command to user-command '" + get_name() + "'!");
  }

  _commands.push_back(cmd);
}

void UserCmd::set_commands(const std::vector<std::shared_ptr<Command>> &commands) 
{ 
  for (const auto &cmd : commands) add_command(cmd); 
}


