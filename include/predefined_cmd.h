#ifndef PREDEFINED_CMD_H
#define PREDEFINED_CMD_H

#include "command.h"
#include "utils.h"

/* Check command */
class CheckCmd : public Command
{
  public:
    CheckCmd() : Command("check", {"$expr"}) {}

    /* Run command */
    void run();
};

/* Set thrust command */
class SetThrustCmd : public Command
{
  public:
    SetThrustCmd() : Command("set_thrust", {"$thrust"}) {}

    /* Run command */
    void run();
};

/* Insert PDS command */
class InsertPdsCmd : public Command
{
  public:
    InsertPdsCmd() : Command("insert_pds", {}) {}

    /* Run command */
    void run(); 
};
#endif
