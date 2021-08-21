#ifndef LOGICAL_EXPR_H
#define LOGICAL_EXPR_H

#include <stack>

#include "error.h"
#include "variable.h"
#include "utils.h"

class LogicalExpr
{
  public:
    LogicalExpr();
    LogicalExpr(const std::string expr, const std::vector<Variable> &variables);

    /* Evaluate logical expression */
    bool eval() const;
    
    /* Get/set */
    void set_expr(const std::string expr);
    void set_variables(const std::vector<Variable> &variables);

    const std::string get_expr() const { return _expr; }
    const std::vector<Variable>& get_variables() const { return _variables; }

  private:
    std::string _expr;
    std::vector<Variable> _variables;
};
#endif
