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

  std::string s = "((a > 180) && (b == 1)) || (c == 3)";
  Utils::eval_logical_expr(s);

  return 0;
}
