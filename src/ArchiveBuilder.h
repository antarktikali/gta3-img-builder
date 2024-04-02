#pragma once
#include <filesystem>

#include "DirWriter.h"
#include "ImgWriter.h"

class ArchiveBuilder
{
  public:
    ArchiveBuilder(DirWriter&, ImgWriter&);
    void AddFile(const std::filesystem::path&);
  private:
    DirWriter& dirWriter;
    ImgWriter& imgWriter;
    size_t totalBlocksWritten = 0;
};

