#include "parser.h"
#include <bitset>
#include <cassert>
#include <cstring>
#include <iostream>
#include <utility>

Memory::Memory(uint64_t size) : m_size(size), m_memory{new uint16_t[size]} {}

Memory::~Memory() {
  if (m_memory)
    delete[] m_memory;
}

Memory::Memory(const Memory &other)
    : m_size(other.m_size), m_memory{new uint16_t[other.m_size]} {
  std::memcpy(m_memory, other.m_memory, sizeof(uint16_t) * m_size);
}

Memory &Memory::operator=(const Memory &other) {
  Memory copy(other);
  std::swap(m_memory, copy.m_memory);
  m_size = copy.m_size;

  return *this;
}

uint16_t Memory::get(uint16_t index) const {
  assert(index < m_size && "index out of bound");
  return m_memory[index];
}

void Memory::set(uint16_t index, uint16_t content) {
  assert(index < m_size && "index out of bound");
  m_memory[index] = content;
}

SASParser::SASParser(std::string filename) : m_file(filename) {
  // parser file here
  if (!m_file.is_open())
    assert(false && "cannot open file");
}

void SASParser::parse(Memory &memory) {
  int line_count = 0;
  std::string line;
  while (std::getline(m_file, line)) {
    assert(line.length() == 20);
    std::string binary_line = line.substr(4, line.length() - 4);
    std::bitset<16> bits(binary_line);
    uint16_t value = static_cast<uint16_t>(bits.to_ulong());

    memory.set(line_count, value);
    ++line_count;
  }

  assert(line_count == 256 && "SAS file should have 256 files");
}

void Memory::dump() const {
  for (uint16_t i = 0; i < m_size; ++i) {
    std::cout << i << " : " << std::hex << (uint32_t)get(i) << "\n";
  }
}
