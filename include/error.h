#ifndef ERROR_H  
#define ERROR_H  

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>

/* Custom exception */
class Error : public std::exception 
{
  public:
    Error() {};
    Error(const std::string &msg) { _msg = "[ERROR]: " + msg; }
    const char* what() const throw () { return _msg.c_str(); }

  protected:
    std::string _msg; // error message
};

class SyntaxError : public Error
{
  public:
    SyntaxError(const std::string &msg) { _msg = "[ERROR] syntax error: " + msg; }
};
#endif
