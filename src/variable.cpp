#include "variable.h"

/**********/
/* Constr */
/**********/
Variable::Variable() {}

Variable::Variable(const std::string name) 
{
  set_name(name); 
}

Variable::Variable(const std::string name, const std::string value) : Variable(name) 
{ 
  set_value(value); 
}


/***********/
/* Get/set */
/***********/
void Variable::set_value(const std::string value) 
{ 
  _value = value; 
}

void Variable::set_name(const std::string name) 
{ 
  if (name.empty())
  {
    throw Error("Variable::set_name: empty variable name!");
  }

  else if (name.front() != '$')
  {
    throw Error("Variable::set_name: illegal variable name '" + name + "'! Missing '$'!");
  }

  else if (name == "$")
  {
    throw Error("Variable::set_name: illegal variable name '" + name + "'!");
  }

  _name = name; 
}


