#include "user_command.h"

/**********/
/* Constr */
/**********/
UserCmd::UserCmd(const size_t n) : Command(n) {}

UserCmd::UserCmd(const std::string name, 
    const std::vector<std::string> &placeholders) : Command(name, placeholders.size()) 
{ 
  set_placeholders(placeholders); 
}

UserCmd::UserCmd(const std::string name, 
    const std::vector<std::string> &placeholders, 
    const std::vector<std::shared_ptr<Command>> &commands) : UserCmd(name, placeholders) 
{ 
  set_commands(commands); 
} 

 
/***************/
/* Run command */
/***************/
void UserCmd::run()
{
  // replace placeholders 
  auto replace_placeholder_with_val = [&] (std::vector<std::string> &args)
  {
    for (size_t i = 0; i < _placeholders.size(); i++)
    {
      for (auto &arg : args) 
      {
	if (_placeholders[i] == arg) arg = this->get_args().at(i);
      }
    }
  };

  for (auto cmd : _commands) 
  {
    auto args = cmd->get_args(); 
    auto replaced_args = cmd->get_args(); 
    replace_placeholder_with_val(replaced_args);

    cmd->set_args(replaced_args);
    cmd->run();
    cmd->set_args(args);
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
    throw Error("UserCmd::add_command: try to add nullptr command to user-command '" + this->get_name() + "'!");
  }

  _commands.push_back(cmd);
}

void UserCmd::set_commands(const std::vector<std::shared_ptr<Command>> &commands) 
{ 
  for (const auto &cmd : commands) add_command(cmd); 
}

void UserCmd::set_placeholders(const std::vector<std::string> &placeholders) 
{ 
  auto it = std::find_if(placeholders.begin(), placeholders.end(), 
      [] (std::string s) { return s.empty() || (!s.empty() && s.front() != '_'); });

  if (it != placeholders.end()) 
  {
    throw Error("UserCmd::set_placeholders: '" + *it + "' not a placeholder! It should begin with '_'!");
  }

  if (placeholders.size() != this->get_args().size())
  {
    throw Error("UserCmd::set_placeholders: command '" + this->get_name() + "' needs " + std::to_string(this->get_args().size()) 
	+ " placeholders, " + std::to_string(placeholders.size()) + " provided!");
  }

  _placeholders = placeholders;
}

