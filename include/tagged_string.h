#ifndef TAGGED_STRING_H
#define  TAGGED_STRING_H

#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <algorithm>

#include "error.h"

/* TaggedString class: it's a std::string starting with a char tag */
template<char T>
class TaggedString
{
  public:
    TaggedString(const std::string str) 
    { 
      if (is_tagged(str)) _string = str; 
      else throw Error("'" + str + "' is not tagged! It should begin with '" + T + "'!");
    }

    friend std::ostream& operator<< (std::ostream& os, const TaggedString &str)
    {
      os << str.to_string();
      return os;
    }

    bool operator==(const TaggedString &str) { return this->to_string() == str.to_string(); }

    /* Convert to std::string */
    std::string to_string() const { return _string; }

    /* Tell if a string is tagged */
    static bool is_tagged(const std::string str) { return (str.front() == T && str.size() > 1); } 

  private:
    std::string _string;
};

/* Placeholder class: TaggedString with "_" */
typedef TaggedString<'_'> Placeholder;

/* Placeholder class: TaggedString with "$" */
typedef TaggedString<'$'> Variable;
#endif
