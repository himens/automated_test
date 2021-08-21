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
    Error();
    Error(const std::string &msg);
    const char* what() const throw ();

  private:
    std::string _msg; // error message
};
#endif
