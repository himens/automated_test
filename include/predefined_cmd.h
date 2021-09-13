#ifndef PREDEFINED_CMD_H
#define PREDEFINED_CMD_H

#include "command.h"
#include "utils.h"
#include "variable.h"

/* Check command */
class CheckCmd : public Command
{
  public:
    CheckCmd() : Command("check", {"$expr"}) {}

    /* Run command */
    void run();
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const;

  private:
    /* Replace variables in expr */
    void replace_variables(std::string &expr) const;
};

/* Set thrust command */
class SetThrustCmd : public Command
{
  public:
    SetThrustCmd() : Command("set_thrust", {"$thrust"}) {}

    /* Run command */
    void run();
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const;
};

/* Insert PDS command */
class InsertPdsCmd : public Command
{
  public:
    InsertPdsCmd() : Command("insert_pds", {}) {}

    /* Run command */
    void run(); 
        
    /* Write command report to file */
    void write_report(std::ofstream &file) const;
};
#endif
