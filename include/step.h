#ifndef STEP_H
#define STEP_H

#include <memory>

#include "command.h"
#include "error.h"
#include "utils.h"

class Step
{
  public:
    Step ();
    Step (const std::string name);
    Step (const std::string name, std::vector<std::shared_ptr<Command>> &commands);

    /* Run step */
    void run();

    /* Write step report to file */
    void write_report(std::ofstream &file) const;
    
    /* Set/get */
    void add_command(const std::shared_ptr<Command> cmd);

    void set_commands(const std::vector<std::shared_ptr<Command>> &commands);
    void set_name(const std::string name); 

    const std::vector<std::shared_ptr<Command>>& get_commands() const { return _commands; }
    const std::string get_name() const { return _name; }

  private:
    std::string _name;
    std::vector<std::shared_ptr<Command>> _commands;  
};
#endif
