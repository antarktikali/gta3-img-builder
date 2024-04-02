#include "ArchiveBuilder.h"

#include <fstream>

#include "Exceptions.h"

ArchiveBuilder::ArchiveBuilder(DirWriter& dirWriter, ImgWriter& imgWriter)
  : dirWriter(dirWriter),
    imgWriter(imgWriter)
{}

void ArchiveBuilder::AddFile(const std::filesystem::path& path)
{
  std::ifstream file(path, std::ios::in | std::ios::binary);
  if (!file.good()) {
    throw CouldNotReadFile(path);
  }
  const size_t blocksInFile = imgWriter.Write(file);
  dirWriter.Write(totalBlocksWritten, blocksInFile, path.filename());
  totalBlocksWritten += blocksInFile;
}

