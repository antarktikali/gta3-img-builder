#pragma once
#include <istream>
#include <ostream>

class ImgWriter
{
  public:
    ImgWriter(std::ostream&, const size_t);
    size_t Write(std::istream&);
  private:
    std::ostream& outStream;
    const size_t blockSize;
};

