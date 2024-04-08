#include "ImgExtracter.h"

#include <fstream>
#include <iostream>

#include "DirEntry.h"
#include "Exceptions.h"

ImgExtracter::ImgExtracter(
    const size_t sectorSize,
    const std::vector<DirEntry>& entries,
    std::istream& imgFile,
    const std::filesystem::path& outDir
)
  : sectorSize(sectorSize),
    entries(entries),
    outDir(outDir),
    imgFile(imgFile)
{}

void ImgExtracter::Extract()
{
  std::filesystem::create_directories(outDir);

  for (const auto& entry : entries) {
    ProcessEntry(entry);
  }
}

std::vector<char> ImgExtracter::ReadSector(const uint32_t sectorIndex)
{
  std::vector<char> bytes(sectorSize, 0);
  imgFile.seekg(sectorIndex * sectorSize);
  imgFile.read(bytes.data(), sectorSize);
  if (!imgFile.good()) {
    throw CouldNotReadSector(std::to_string(sectorIndex));
  }
  return bytes;
}

void ImgExtracter::ProcessEntry(const DirEntry& entry)
{
  std::filesystem::path outFile = outDir / entry.fileName;
  if (std::filesystem::exists(outFile)) {
    throw FileAlreadyExists(outFile);
  }

  std::ofstream out(outFile, std::ios::out | std::ios::binary);
  for (uint32_t i = 0; i < entry.sectorCount; i++) {
    const auto& bytes = ReadSector(entry.sectorStart + i);
    out.write(bytes.data(), std::min(sectorSize, bytes.size()));
  }

  std::cout << outFile.string() << std::endl;
}

