#include "step.h"

/************/
/* Run step */
/************/
void Step::run()
{
  if (_commands.size() > 0)
  {
    Utils::print_banner("Run step '" + _name + "':");

    for (auto cmd : _commands)
    {
      std::cout << "Run command '" << cmd->get_name() << "':\n";
      cmd->run();
      std::cout << "\n";
    }
  }
  else
  {
    std::cout << "Step '" << _name << "' does not contain any command!\n";
  } 
}


/*****************************/
/* Write step report to file */
/*****************************/
void Step::write_report(std::ofstream &file) const
{
  if (!file.is_open())
  {
    throw Error("cannot open report file!");
  }

  file << "## Report of step: '" << _name << "' \n";

  if (_commands.size() > 0)
  {
    for (auto cmd : _commands) cmd->write_report(file);
  }
  else
  {
    file << "### Step: '" << _name << "' does not contain any command! \n";
  }
}


/***********/
/* Set/get */
/***********/
void Step::add_command(const std::shared_ptr<Command> cmd) 
{
  if (cmd == nullptr) 
  {
    throw Error("Step::add_command: try to add nullptr to step '" + _name + "'!");
  }

  _commands.push_back(cmd);
}

void Step::set_commands(const std::vector<std::shared_ptr<Command>> &commands) 
{ 
  for (const auto &cmd : commands) add_command(cmd); 
}

void Step::set_name(const std::string name) 
{ 
  if (name.empty()) 
  {
    throw Error("Step::set_name: name is empty!");
  }

  _name = name; 
}


