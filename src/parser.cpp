#include "parser.h"
#include <bitset>
#include <cassert>
#include <cstring>
#include <elfio/elfio_dump.hpp>
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

SASParser::SASParser(const char *filename) : ::Parser(), m_file(filename) {
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

ELFParser::ELFParser(const char *filename) : ::Parser(), m_reader() {
  bool status = m_reader.load(filename);
  assert(m_reader.get_class() == ELFIO::ELFCLASS32 &&
         "must be a 32 bit elf record");
  assert(status);
}

void ELFParser::parseTextSection(const ELFIO::section *section,
                                 Memory &memory) {
  const uint16_t *pdata =
      reinterpret_cast<const uint16_t *>(section->get_data());
  if (!pdata) {
    assert(0 && "empty section data! Parser Error");
    return;
  }

  assert(section->get_size() % 2 == 0 &&
         "must be true! Becuase these are only instruction encodings!");
  ELFIO::Elf_Xword i;
  for (i = 0; i < std::min(section->get_size() / 2, ELFIO::MAX_DATA_ENTRIES);
       ++i) {
    std::bitset<16> instruction(m_reader.get_convertor()(pdata[i]));
    uint16_t instr = instruction.to_ulong();
    memory.set(i, instr);
  }
}

void ELFParser::parse(Memory &memory) {
  // this may be game over
  // ELFIO::dump::section_data(std::cout, m_reader);
  ELFIO::dump::section_headers(std::cout, m_reader);
  for (ELFIO::Elf_Half i = 0; i < m_reader.sections.size();
       ++i) { // For all sections
    const ELFIO::section *sec = m_reader.sections[i];

    if (sec->get_name() == ".text") {
      std::cout << "Loading .text into 0x00! Ignoring ELF" << std::endl;
      parseTextSection(sec, memory);
    }
  }
}

void Parser::parse(Memory &memory) {
  assert(0 && "this should be unreachable");
}

std::unique_ptr<Parser> createParser(const char *filename, FileType type) {
  switch (type) {
  case ELF:
    return std::make_unique<ELFParser>(filename);
  case SAS:
    return std::make_unique<SASParser>(filename);
  default:
    assert(0 && "this is game over");
  }
}
