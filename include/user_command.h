#ifndef USER_CMD_H
#define USER_CMD_H

#include <memory>

#include "command.h"

/* UserCmd class: define a user-command which can be seen as new command with its own arguments.
   It's a set of already existing commands plus placeholders which stand for user-command arguments. 
 */
class UserCmd : public Command
{
  public:
    UserCmd() {}
    UserCmd(const size_t n) : Command(n) {}

    UserCmd(const std::string name, 
	    const std::vector<std::string> &placeholders) : Command(name, placeholders.size()) 
    { 
      set_placeholders(placeholders); 
    }

    UserCmd(const std::string name, 
  	    const std::vector<std::string> &placeholders, 
	    const std::vector<std::shared_ptr<Command>> &commands) : UserCmd(name, placeholders) 
    { 
      set_commands(commands); 
    } 

    /* Run command */
    void run();
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const;

    /* Set/get */
    void add_command(const std::shared_ptr<Command> cmd);
    void set_commands(const std::vector<std::shared_ptr<Command>> &commands);
    void set_placeholders(const std::vector<std::string> &placeholders);
    
    const std::vector<std::shared_ptr<Command>>& get_commands() const { return _commands; }
    const std::vector<std::string>& get_placeholders() const { return _placeholders; }

  private:
    std::vector<std::shared_ptr<Command>> _commands;
    std::vector<std::string> _placeholders;
};
#endif
