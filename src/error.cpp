#include "error.h"

/**********/
/* Constr */
/**********/
Error::Error() {};

Error::Error(const std::string &msg) 
{ 
  _msg = "[ERROR]: " + msg; 
}

const char* Error::what() const throw () 
{ 
  return _msg.c_str(); 
}
