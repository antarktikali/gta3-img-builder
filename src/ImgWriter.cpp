#include "ImgWriter.h"

#include <iomanip>

ImgWriter::ImgWriter(std::ostream& outStream, const size_t blockSize)
  : outStream(outStream),
    blockSize(blockSize)
{}

size_t ImgWriter::Write(std::istream& inStream)
{
  const auto initialPosition = outStream.tellp();
  outStream << inStream.rdbuf();
  const auto bytesWritten = outStream.tellp() - initialPosition;

  auto blocksWritten = bytesWritten / blockSize;
  const bool needsFilling = (bytesWritten % blockSize) != 0;

  if (needsFilling) {
    blocksWritten++;
    const auto numOfFillBytes = blockSize - (bytesWritten % blockSize);
    outStream.fill('\0');
    outStream << std::setw(numOfFillBytes) << '\0';
  }

  return blocksWritten;
}

