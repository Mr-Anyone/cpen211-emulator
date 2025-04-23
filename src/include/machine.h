#ifndef MACHINE_H
#define MACHINE_H

#include "parser.h"
#include <cstdint>
#include <string>
#include <vector>

class Machine {
public:
  Machine(std::string filename, uint16_t size);

  void runTilHalt();

  void printRegs();

private:
  // void executePC();

  uint16_t m_regs[8];
  uint16_t m_pc;
  Memory m_memory;
};

#endif
