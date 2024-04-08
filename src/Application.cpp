#include "Application.h"

#include <iostream>
#include <fstream>

#include "ArchiveBuilder.h"
#include "DirEntry.h"
#include "DirWriter.h"
#include "Exceptions.h"
#include "FileListReader.h"
#include "ImgExtracter.h"
#include "ImgWriter.h"

Application::Application(int argc, char* argv[])
  : exeName(argv[0]), arguments(GetArguments(argc, argv))
{}

std::vector<std::string> Application::GetArguments(int argc, char* argv[])
{
  std::vector<std::string> arguments;
  for (int i = 1; i < argc; i++) {
    arguments.emplace_back(argv[i]);
  }
  return arguments;
}

void Application::PrintBasicHelp() const
{
  std::cout << "Usage: " << exeName << " <command>" << std::endl;

  std::cout <<
  std::endl << "command:" <<
  std::endl << "  build:   Build a new .img archive." <<
  std::endl << "  extract: Extract files from an existing .img archive." <<
  std::endl;
}

void Application::PrintBuildHelp() const
{
  std::cout << "Usage: " << exeName << " build " <<
            "<files_list_file> <dir_file> <img_file>" << std::endl;

  std::cout <<
  std::endl << "  <files_list_file>: Text file containing paths to the files to add to the archive." <<
  std::endl << "  <dir_file>:        Path of the new .dir file to write." <<
  std::endl << "  <img_file>:        Path of the new .img file to write." <<
  std::endl;
}

void Application::PrintExtractHelp() const
{
  std::cout << "Usage: " << exeName << " extract " <<
            "<dir_file> <img_file> <output_directory>" << std::endl;

  std::cout <<
  std::endl << "  <dir_file>:         Path of the .dir file to read." <<
  std::endl << "  <img_file>:         Path of the .img file to read." <<
  std::endl << "  <output_directory>: Directory where the extracted files should be written to." <<
  std::endl;
}

bool Application::ContainsHelpArgument() const
{
  for (const auto& argument : arguments) {
    if (IsHelpArgument(argument)) {
      return true;
    }
  }
  return false;
}

bool Application::IsHelpArgument(const std::string& argument)
{
  return (argument == "--help" || argument == "-h");
}

Application::Command Application::GetCommand() const
{
  if (arguments.size() < 1) {
    return Command::INVALID;
  } else if (arguments[0] == "build") {
    return Command::BUILD;
  } else if (arguments[0] == "extract") {
    return Command::EXTRACT;
  } else if (IsHelpArgument(arguments[0])) {
    return Command::BASIC_HELP;
  }
  return Command::INVALID;
}

int Application::DoBuild() const
{
  if (ContainsHelpArgument()) {
    PrintBuildHelp();
    return 0;
  } else if (arguments.size() != 4) {
    PrintBuildHelp();
    return 1;
  }

  const auto filesListToRead = arguments[1];
  const auto dirToWrite = arguments[2];
  const auto imgToWrite = arguments[3];

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
  ImgWriter imgWriter(imgFile, SECTOR_SIZE);
  ArchiveBuilder builder(dirWriter, imgWriter);

  for (const auto& file : filesList) {
    builder.AddFile(file);
  }

  return 0;
}

int Application::DoExtract() const
{
  if (ContainsHelpArgument()) {
    PrintExtractHelp();
    return 0;
  } else if (arguments.size() != 4) {
    PrintExtractHelp();
    return 1;
  }

  const auto dirToRead = arguments[1];
  const auto imgToRead = arguments[2];
  const auto outputDirectory = arguments[3];

  const auto entries = DirEntry::ParseDirFile(dirToRead);
  std::ifstream imgFile(imgToRead, std::ios::in | std::ios::binary);
  if (!imgFile.good()) {
    throw CouldNotReadFile(imgToRead);
  }

  ImgExtracter extracter(SECTOR_SIZE, entries, imgFile, outputDirectory);
  extracter.Extract();

  return 0;
}

int Application::Run()
{
  try {
    switch (GetCommand()) {
      case Command::BUILD:
        return DoBuild();
      case Command::EXTRACT:
        return DoExtract();
      case Command::BASIC_HELP:
        PrintBasicHelp();
        return 0;
      case Command::INVALID:
        PrintBasicHelp();
        return 1;
    }
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }

  return 0;
}

