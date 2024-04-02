#include "FileListReader.h"

#include <fstream>
#include <unordered_set>

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

  if (HasConflictingFilenames(pathList)) {
    throw ConflictingFilenames();
  }

  return pathList;
}

bool FileListReader::HasConflictingFilenames(const std::vector<std::filesystem::path>& paths)
{
  std::unordered_set<std::filesystem::path> filenames;
  for (const auto& path : paths) {
    if (filenames.contains(path.filename())) {
      return true;
    }
    filenames.insert(path.filename());
  }

  return false;
}

