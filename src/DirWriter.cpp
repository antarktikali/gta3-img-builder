#include "DirWriter.h"

#include <cstring>

#include "Exceptions.h"

DirWriter::DirWriter(std::ostream& outStream)
  : outStream(outStream)
{}

void DirWriter::Write(const uint32_t startingBlock, const uint32_t blockCount, const std::string& fileName)
{
  char fileNameBuf[24] = {0};
  // Should have 1 extra space for null termination
  if (fileName.length() >= 24) {
    throw FileNameTooLong(fileName);
  }

  outStream.write(reinterpret_cast<char const*>(&startingBlock), sizeof(startingBlock));
  outStream.write(reinterpret_cast<char const*>(&blockCount), sizeof(blockCount));
  strcpy(fileNameBuf, fileName.c_str());
  outStream.write(fileNameBuf, sizeof(fileNameBuf));
}

