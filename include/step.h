#ifndef STEP_H
#define STEP_H

#include "command.h"

class Step
{
  public:
    Step () {}
    Step (const std::string name) { set_name(name); }
    Step (const std::string name, std::vector<std::shared_ptr<Command>> &commands) : Step(name) { set_commands(commands); }

    /* Run step */
    void run() 
    {
      if (_commands.size() == 0)
      {
	std::cout << "Step '" << _name << "' has not commands to run!\n";
	return;
      }

      Utils::print_banner("Run step '" + _name + "':");
      
      for (auto cmd : _commands)
      {
	std::cout << "Run command '" << cmd->get_name() << "':\n";
	cmd->run();
	std::cout << "\n";
      }
    }

    /* Set/get */
    void set_name(const std::string name) 
    { 
      if (name.empty()) 
      {
	throw Error("Step::set_name: name is empty!");
      }

      _name = name; 
    }
    const std::string get_name() const { return _name; }
    
    void set_commands(const std::vector<std::shared_ptr<Command>> &commands) { _commands = commands; }
    const std::vector<std::shared_ptr<Command>> &get_commands() const { return _commands; }

  private:
    std::string _name;
    std::vector<std::shared_ptr<Command>> _commands;  
};
#endif
