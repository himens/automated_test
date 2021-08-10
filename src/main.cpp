#include "test.h"

int main()
{
  try
  {
    Test test("example.test");
    test.run();
  }
  catch (const Error &e) 
  { 
    std::cout << e.what() << "\n"; 
  }

  //std::string s = "((@ground_speed > 180) && (pp_accept == 1)) || (@config_check == 3)";
  //auto tokens = Utils::tokens(s, '(');
  //for (auto &t : tokens) std::cout << t << " - ";
  //std::cout << "\n";

  return 0;
}
