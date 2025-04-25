#include "machine.h"
#include "parser.h"
#include <iostream>

int main(int argc, char *argv[]) {
  if (argc != 2) {
    std::cerr << "Please provide a path to assembly" << std::endl;
    return -1;
  }

  std::cout << "Running: " << argv[1] << std::endl;

  Machine machine(argv[1], /*size=*/256);

  std::cout << "Output: " << std::endl;
  machine.runTilHalt();
  machine.printRegs();
  return 0;
}
