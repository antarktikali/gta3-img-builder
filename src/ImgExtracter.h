#pragma once

#include <filesystem>
#include <istream>
#include <vector>

#include "DirEntry.h"

class ImgExtracter
{
  public:
    ImgExtracter(
        const size_t,
        const std::vector<DirEntry>&,
        std::istream&,
        const std::filesystem::path&
    );
    void Extract();
  private:
    std::vector<char> ReadSector(const uint32_t);
    void ProcessEntry(const DirEntry&);
    std::filesystem::path GetOutputPathForEntry(const DirEntry&) const;

    const size_t sectorSize;
    const std::vector<DirEntry>& entries;
    const std::filesystem::path outDir;
    std::istream& imgFile;
};

