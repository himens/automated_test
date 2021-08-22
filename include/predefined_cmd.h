#ifndef PREDEFINED_CMD_H
#define PREDEFINED_CMD_H

#include "command.h"
#include "utils.h"
#include "variable.h"
#include "logical_expr.h"

/* Check command */
class CheckCmd : public Command
{
  public:
    CheckCmd();

    /* Run command */
    void run();
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const;
   
  private: 
    LogicalExpr _expr;
};

/* Set thrust command */
class SetThrustCmd : public Command
{
  public:
    SetThrustCmd();

    /* Run command */
    void run();
    
    /* Write command report to file */
    void write_report(std::ofstream &file) const;
    
  private:
    int _thrust = 0; 
};

/* Insert PDS command */
class InsertPdsCmd : public Command
{
  public:
    InsertPdsCmd();

    /* Run command */
    void run(); 
        
    /* Write command report to file */
    void write_report(std::ofstream &file) const;

  private:
    bool _pds_inserted = false; 
};
#endif
