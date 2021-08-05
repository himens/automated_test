#ifndef ERROR_H  
#define ERROR_H  

#include <iostream>
#include <sstream>
#include <string>
#include <stdexcept>


/* Custom exception */
class Error : public std::exception {
  public:
    Error() {}
    Error(const char* msg) { set_msg(msg); }
    Error(const std::string &msg) { set_msg(msg); }
    const char* what() const throw () { return _msg.c_str(); }

    void set_msg(const std::string msg) { _msg = "[ERROR]: " + msg; }

  private:
    std::string _msg = "Error"; // error message
};
#endif
