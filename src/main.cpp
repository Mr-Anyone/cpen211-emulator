#include "machine.h"
#include "parser.h"
#include <iostream>

int commandLineEntry(char *argv[]) {
  std::cout << "Running: " << argv[1] << std::endl;

  Machine machine(argv[1], SAS, /*size=*/0xfff);

  std::cout << "Output: " << std::endl;
  machine.runTilHalt();
  machine.printRegs();
  return 0;
}

int main(int argc, char *argv[]) {
  if (argc == 2) {
    return commandLineEntry(argv);
  }

  Machine machine("./src/test_programs/elf_program", SAS, std::numeric_limits<uint16_t>::max());
  machine.runTilHalt();
  return 0;
}
