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


  return 0;
}
