#include "function_store.h"

/**********/
/* detail */
/**********/
namespace detail
{
  // convert string to digit
  auto is_digit = [] (const std::string str)
  {
    return !str.empty() && (str.find_first_not_of("0123456789.") == std::string::npos);
  }; 
};


/*********/
/* Check */
/*********/
bool FunctionStore::check(const std::string a, const std::string b)
{
  bool is_passed = false;

  if (detail::is_digit(a) && detail::is_digit(b)) 
    is_passed = std::abs(std::stof(a) - std::stof(b)) < 1e-5;

  else if (!detail::is_digit(a) && !detail::is_digit(b)) 
    is_passed = a == b;

  else 
    std::cout << "WARNING: checking data of different types!" << std::endl;

  std::cout << "check " << (is_passed ? "PASSED" : "FAILED") << std::endl;

  return is_passed;
}


/**************/
/* Set thrust */
/**************/
void FunctionStore::set_thrust(const std::string value) 
{ 
  int thrust = detail::is_digit(value) ? std::stoi(value) : 0;
  if (!detail::is_digit(value)) std::cout << "WARNING: 'set_thrust': value '" << value << "' is not a number! \n";
  
  std::cout << "set_thrust to " << thrust << "\n"; 
}


/**************/
/* Insert PDS */
/**************/
void FunctionStore::insert_pds() { std::cout << "insert_pds! \n"; }

