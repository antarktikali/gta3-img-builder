#pragma once

#include <cstdint>
#include <filesystem>
#include <vector>

struct DirEntry
{
  static std::vector<DirEntry> ParseDirFile(const std::filesystem::path&);

  uint32_t sectorStart;
  uint32_t sectorCount;
  char fileName[24];
};

