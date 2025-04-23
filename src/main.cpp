#include "parser.h"
#include <iostream>

int main() {
  std::cout << "Hello World!" << std::endl;

  Memory memory(256);
  SASParser parser("./src/test_programs/fib.sas");
  parser.parse(memory);

  return 0;
}
