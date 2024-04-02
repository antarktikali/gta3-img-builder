#pragma once

#include <filesystem>
#include <vector>

namespace FileListReader
{
  std::vector<std::filesystem::path> Read(const std::filesystem::path&);
  bool HasConflictingFilenames(const std::vector<std::filesystem::path>&);
};

