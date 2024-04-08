#include "FileListReader.h"

#include <fstream>

#include "Exceptions.h"

std::vector<std::filesystem::path> FileListReader::Read(const std::filesystem::path& path)
{
  std::ifstream listFile(path, std::ios::in);
  if (!listFile.good()) {
    throw CouldNotReadFilesList(path);
  }

  std::vector<std::filesystem::path> pathList;
  for (std::string line; std::getline(listFile, line);) {
    if (0 < line.length()) {
      pathList.push_back(line);
    }
  }

  return pathList;
}

