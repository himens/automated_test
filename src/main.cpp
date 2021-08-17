#include "test.h"

int main(int argc, char **argv)
{
  std::string test_file;

  switch (argc) 
  {
    case 2: 
      test_file = argv[1]; 
      break;

    default:  
      std::cout << "Usage: ./test.exe [test file] \n";
      return 0;
  }

  try
  {
    Test test(test_file);
    test.run();
  }
  catch (const Error &e) 
  { 
    std::cout << e.what() << "\n"; 
  }

  return 0;
}
