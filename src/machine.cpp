#include "machine.h"
#include "parser.h"
#include <cassert>
#include <iostream>

Machine::Machine(std::string filename, uint16_t size)
    : m_memory(size), m_regs{0, 0, 0, 0, 0, 0, 0, 0}, m_pc(0) {
  SASParser parser(filename);
  parser.parse(m_memory);
}

void Machine::runTilHalt() {
  while (true) {
    // parser instruction
    uint16_t instruction = m_memory.get(m_pc);
    // HALT
    if (instruction == 0b1110000000000000)
      break;

    uint16_t opcode = (instruction >> 13) & 0b111;
    switch (opcode) {
    case 0b110: {
      // move type instruction
      uint16_t op = (instruction >> 11) & 0b11;
      switch (op) {
      case 0b10:
      case 0b00:
      default:
        assert(false && "not sure what instruction this is");
      }
    }
    case 0b101: {
      // alu instruction
    }
    case 0b011: {
      // ldr
    }
    case 0b100: {
      // str
    }
    case 0b010: {
      // BL,BX,BLX type
    }
    case 0b001: {
      // B, BEQ, BNE, BLT, BLE
    }

    default:
      assert(false && "you have reached impossible to get instruction");
    }
  }
}

void Machine::printRegs() {
  for (int i = 0; i < 8; ++i) {
    std::cout << "R" << i << "  = " << m_regs[i] << "\t";
  }
  std::cout << "PC = " << m_pc << "\t";

  std::cout << "\n";
}
