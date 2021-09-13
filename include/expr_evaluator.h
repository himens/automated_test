#ifndef EXPR_EVALUATOR_H
#define EXPR_EVALUATOR_H

#include <stack>

#include "error.h"
#include "utils.h"

class ExprEvaluator
{
  public:
    ExprEvaluator();

    /* Evaluate logical expression */
    bool eval(const std::string expr) const;
};
#endif
