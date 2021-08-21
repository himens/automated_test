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
      std::cout << "Usage: " + std::string{argv[0]} + " [test file] \n";
      return 0;
  }

  try
  {
    Test test{"test"};
    test.read_test_file(test_file);
    test.run();
  }
  catch (const Error &e) 
  { 
    std::cout << e.what() << "\n"; 
  }

  return 0;
}
