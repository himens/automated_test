#ifndef STEP_H
#define STEP_H

class Step
{
  public:
    Step () {}
    Step (const std::string name) { set_name(name); }
    Step (const std::string name, std::vector<std::shared_ptr<Command>> &commands) : Step(name) { set_commands(commands); }

    /* Run step */
    void run()
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

    /* Write step report to file */
    void write_report(std::ofstream &file) const
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

    /* Set/get */
    void add_command(const std::shared_ptr<Command> cmd) 
    {
      if (cmd == nullptr) 
      {
	throw Error("Step::add_command: try to add nullptr to step '" + _name + "'!");
      }

      _commands.push_back(cmd);
    }

    void set_commands(const std::vector<std::shared_ptr<Command>> &commands) 
    { 
      for (const auto &cmd : commands) add_command(cmd); 
    }

    void set_name(const std::string name) 
    { 
      if (name.empty()) 
      {
	throw Error("Step::set_name: name is empty!");
      }

      _name = name; 
    }

    const std::vector<std::shared_ptr<Command>>& get_commands() const { return _commands; }
    const std::string get_name() const { return _name; }

  private:
    std::string _name;
    std::vector<std::shared_ptr<Command>> _commands;  
};
#endif
