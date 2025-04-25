#include "machine.h"
#include "parser.h"
#include <bitset>
#include <cassert>
#include <ios>
#include <iostream>

Machine::Machine(std::string filename, uint16_t size)
    : m_memory(size), m_regs{0, 0, 0, 0, 0, 0, 0, 0}, m_pc(0) {
  SASParser parser(filename);
  parser.parse(m_memory);
}

constexpr uint16_t sximm5(uint16_t x) {
  constexpr int bit_width = 5;
  constexpr uint16_t sign_bit = 1u << (bit_width - 1); // 0b10000
  constexpr uint16_t mask = (1u << bit_width) - 1;     // 0b11111

  x &= mask;                                      // keep only 5 bits
  int16_t signed_val = (x ^ sign_bit) - sign_bit; // sign-extend to int16_t
  return static_cast<uint16_t>(signed_val);       // reinterpret as uint16_t
}

int16_t sximm8(uint8_t imm8) { return (int16_t)((uint8_t)imm8); }

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
      case 0b10: {
        // MOV16ri
        uint16_t rn = (instruction >> 8) & 0b111;
        m_regs[rn] = sximm8(instruction & 0xFF);
        break;
      }
      case 0b00: {
        // MOV16rr, sh
        uint16_t rd = (instruction >> 5) & 0b111;

        uint16_t rm = instruction & 0b111;
        uint16_t shift = (instruction >> 3) & 0b11;
        assert(shift == 0 && "don't support this for now");
        m_regs[rd] = m_regs[rm];
        break;
      }
      default:
        assert(false && "not sure what instruction this is");
      }

      ++m_pc;
      break;
    }
    case 0b101: {
      // alu instruction
      uint16_t op = (instruction >> 11) & 0b11;

      uint16_t rd = (instruction >> 5) & 0b111;
      uint16_t rn = (instruction >> 8) & 0b111;
      uint16_t rm = instruction & 0b111;
      uint16_t shift = (instruction >> 3) & 0b11;
      assert(shift == 0 && "don't support this for now");
      switch (op) {
      case 0b00: {
        // add
        m_regs[rd] = m_regs[rn] + m_regs[rm];
        break;
      }
      case 0b01: {
        // cmp
        uint16_t subtract = m_regs[rn] - m_regs[rm];
        if (subtract == 0) {
          z = 0;
        }
        if (subtract >> 15 == 1) {
          n = 1;
        }

        // assign V = (Ain[15]^negativeBin[15]) ? 0: (out[15]^Ain[15]); //
        // TODO: write a unit test?
        int16_t negative_b = -static_cast<int16_t>(m_regs[rm]);
        int16_t high_b_bits = negative_b >> 15;
        int16_t high_a_bits = m_regs[rn] >> 15;
        int16_t high_subtract_bits = subtract >> 15;
        v = (high_a_bits ^ negative_b) ? 0 : (high_subtract_bits ^ high_a_bits);
        break;
      }
      case 0b10: {
        // and
        m_regs[rd] = m_regs[rn] & m_regs[rm];
        break;
      }
      case 0b11: {
        // MVN
        m_regs[rd] = ~m_regs[rn];
        break;
      }
      default:
        assert(false && "unknown alu op code");
      }

      ++m_pc;
      break;
    }
    case 0b011: {
      // ldr
      uint16_t rd = (instruction >> 5) & 0b111;
      uint16_t rn = (instruction >> 8) & 0b111;
      int16_t sximm5 = (instruction) & 0b11111;
      m_regs[rd] = m_memory.get(sximm5 + m_regs[rn]);
      std::cout << "it is: " << sximm5 << std::endl;

      ++m_pc;
      break;
    }
    case 0b100: {
      uint16_t rd = (instruction >> 5) & 0b111;
      uint16_t rn = (instruction >> 8) & 0b111;
      int16_t sximm5 = (instruction) & 0b11111;
      m_memory.set(m_regs[rn] + sximm5, m_regs[rd]);

      ++m_pc;
      // str
      break;
    }
    case 0b010: {
      uint16_t op = (instruction >> 8) & 0b11111;
      int16_t imm8 = sximm8(instruction & 0xFF);
      switch (op) {
      case 0b11111: {
        // BL
        m_regs[7] = m_pc + 1;
        m_pc = imm8 + m_pc + 1;
        break;
      }
        // TODO: what aboud BLX, BX
      default:
        assert(false && "don't know what to do bro");
      }
      // BL,BX,BLX type
      break;
    }
    case 0b001: {
      // B, BEQ, BNE, BLT, BLE
      uint16_t cond = (instruction >> 8) & 0b111;
      int16_t imm8 = sximm8(instruction & 0xFF);
      assert(((instruction >> 11) & 0b11) == 0b00);

      switch (cond) {
      case 0b000: {
        // B
        m_pc = 1 + (int16_t)(m_pc) + imm8;
        break;
      }
      case 0b001: {
        // BEQ
        if (z == 1) {
          m_pc = 1 + (int16_t)(m_pc) + imm8;
        } else {
          m_pc = m_pc + 1;
        }
        break;
      }
      case 0b010: {
        // BNE
        if (z == 0) {
          m_pc = 1 + (int16_t)(m_pc) + imm8;
        } else {
          m_pc = m_pc + 1;
        }
        break;
      }
      case 0b011: {
        // BLT
        if (n != v) {
          m_pc = 1 + (int16_t)(m_pc) + imm8;
        } else {
          m_pc += 1;
        }
        break;
      }
      case 0b100: {
        // BLE
        if (n != v && z == 1) {
          m_pc = 1 + (int16_t)(m_pc) + imm8;
        } else {
          m_pc += 1;
        }
        break;
      }

      default:
        assert(false && "unknown instruciton");
      }

      break;
    }
    default:
      std::bitset<3> bin_opcode(opcode);
      m_memory.dump();
      std::cerr << "the opcode is: " << bin_opcode << opcode << std::endl;
      printRegs();
      assert(false && "you have reached impossible to get instruction");
    }
  }
}

void Machine::printRegs() {
  for (int i = 0; i < 8; ++i) {
    std::cout << "R" << i << "=" << m_regs[i] << "\t";
  }
  std::cout << "PC=" << m_pc << "\t";

  std::cout << "\n";
}
