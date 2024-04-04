#include "DirEntry.h"

#include <fstream>
#include <iostream>

#include "Exceptions.h"

std::vector<DirEntry> DirEntry::ParseDirFile(const std::filesystem::path& path)
{
  std::ifstream dirFile(path, std::ios::in | std::ios::binary);
  if (!dirFile.good()) {
    throw CouldNotReadFile(path);
  }

  std::vector<DirEntry> entries;
  while (dirFile.peek() != std::char_traits<char>::eof())
  {
    DirEntry entry;
    dirFile.read(reinterpret_cast<char*>(&entry), sizeof(DirEntry));

    if (dirFile.eof()) {
      throw UnexpectedEof(path);
    }

    entries.push_back(entry);
  }

  return entries;
}

