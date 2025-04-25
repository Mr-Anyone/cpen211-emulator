#include "machine.h"
#include "parser.h"
#include <iostream>

int main() {
  std::cout << "Hello World!" << std::endl;

  Machine machine("./src/test_programs/fib.sas", /*size=*/256);
  machine.runTilHalt();
  machine.printRegs();
  return 0;
}
