#ifndef PARSER_H
#define PARSER_H

#include "elfio/elfio.hpp"
#include <cstdint>
#include <fstream>
#include <memory>

enum FileType { ELF, SAS };

// FIXME: move this into a different file
class Memory {
public:
  Memory(uint64_t size);
  Memory(const Memory &other);
  ~Memory();
  Memory &operator=(const Memory &other);

  uint16_t get(uint16_t index) const;
  void set(uint16_t index, uint16_t content);

  void dump() const;

private:
  uint16_t *m_memory;
  uint64_t m_size;
};

class Parser {
public:
  virtual void parse(Memory &memory);
};

class SASParser : public Parser {
public:
  SASParser(const char *filename);

  virtual void parse(Memory &memory) override;

private:
  std::ifstream m_file;
};

class ELFParser : public Parser {
public:
  ELFParser(const char *filename);

  virtual void parse(Memory &memory) override;

private:
  void parseTextSection(const ELFIO::section *section, Memory&memory);
  ELFIO::elfio m_reader;
};

std::unique_ptr<Parser> createParser(const char *filename, FileType type);
#endif
