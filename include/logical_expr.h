#ifndef LOGICAL_EXPR_H
#define LOGICAL_EXPR_H

#include "error.h"
#include "variable.h"
#include "utils.h"

class LogicalExpr
{
  public:
    LogicalExpr() {};
    LogicalExpr(const std::string expr, const std::vector<Variable> &variables) 
    {
      _expr = expr;
      _variables = variables;
    };

    /* Evaluate logical expression */
    bool eval();
    
  private:
    std::string _expr;
    std::vector<Variable> _variables;
};
#endif
