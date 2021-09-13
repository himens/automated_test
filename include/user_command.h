#ifndef USER_CMD_H
#define USER_CMD_H

#include <memory>

#include "command.h"

/* UserCmd class: a set of existing commands. It can be seen as a new command with its own arguments.
   Example: 
   > UserCmd my_cmd{"my_cmd", {"$x", "$y"}}; // my_cmd takes two input arguments
   >
   > std::shared_ptr<Command> cmd1 = std::make_shared<SetThrustCmd>();
   > std::shared_ptr<Command> cmd2 = std::make_shared<SetThrustCmd>();
   > std::shared_ptr<Command> cmd3 = std::make_shared<InsertPdsCmd>();
   >
   > cmd1->set_args("$x");
   > cmd2->set_args("$y");
   >
   > usr_cmd->set_commands({cmd1, cmd2, cmd3});
   > usr_cmd->set_args_values({"1", "2"});
   > usr_cmd->run(); // it will set thrust to 1, 2 and then insert pds 
 */
class UserCmd : public Command
{
  public:
    UserCmd(const std::string name, 
	    const std::vector<std::string> &args_names);

    UserCmd(const std::string name, 
  	    const std::vector<std::string> &args_names, 
	    const std::vector<std::shared_ptr<Command>> &commands);

    /* Run command */
    void run();
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const;

    /* Set/get */
    void add_command(const std::shared_ptr<Command> cmd);

    void set_commands(const std::vector<std::shared_ptr<Command>> &commands);
    
    const std::vector<std::shared_ptr<Command>>& get_commands() const { return _commands; }

  private:
    std::vector<std::shared_ptr<Command>> _commands;
};
#endif
