#ifndef PARSER_H
#define PARSER_H

#include <cstdint>
#include <fstream>
#include <string>

// FIXME: move this into a different file
class Memory {
public:
  Memory(uint64_t size);
  Memory(const Memory &other);
  ~Memory();
  Memory &operator=(const Memory &other);

  uint16_t get(uint16_t index);
  void set(uint16_t index, uint16_t content);

private:
  uint16_t *m_memory;
  uint64_t m_size;
};

class SASParser {
public:
  SASParser(std::string filename);

  void parse(Memory &memory);

private:
  std::ifstream m_file;
};


#endif
