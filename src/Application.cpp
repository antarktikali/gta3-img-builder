#include "Application.h"

#include <fstream>

#include "ArchiveBuilder.h"
#include "DirWriter.h"
#include "Exceptions.h"
#include "FileListReader.h"
#include "ImgWriter.h"

Application::Application(
    const std::string& filesListToRead,
    const std::string& dirToWrite,
    const std::string& imgToWrite
) : filesListToRead(filesListToRead),
    dirToWrite(dirToWrite),
    imgToWrite(imgToWrite)
{}

void Application::Run()
{
  const auto filesList = FileListReader::Read(filesListToRead);

  if (std::filesystem::exists(dirToWrite)) {
    throw FileAlreadyExists(dirToWrite);
  }
  if (std::filesystem::exists(imgToWrite)) {
    throw FileAlreadyExists(imgToWrite);
  }

  std::ofstream dirFile(dirToWrite, std::ios::out | std::ios::binary);
  std::ofstream imgFile(imgToWrite, std::ios::out | std::ios::binary);
  DirWriter dirWriter(dirFile);
  ImgWriter imgWriter(imgFile, 2048);
  ArchiveBuilder builder(dirWriter, imgWriter);

  for (const auto& file : filesList) {
    builder.AddFile(file);
  }
}

