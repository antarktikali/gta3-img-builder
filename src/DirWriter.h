#pragma once
#include <cstdint>
#include <ostream>

class DirWriter
{
  public:
    DirWriter(std::ostream&);
    void Write(const uint32_t, const uint32_t, const std::string&);
  private:
    std::ostream& outStream;
};

