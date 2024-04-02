#pragma once
#include <stdexcept>

#define DEFINE_EXCEPTION(name, description) \
class name : public std::runtime_error \
{ \
  public: \
    name() : runtime_error(description) {} \
    name(const::std::string& extraInfo) \
      : runtime_error(std::string(description).append(": ").append(extraInfo)) {} \
};

DEFINE_EXCEPTION(FileNameTooLong, "Given filename is too long");
DEFINE_EXCEPTION(CouldNotReadFile, "Could not read the given file");
DEFINE_EXCEPTION(CouldNotReadFilesList, "Could not find files list");
DEFINE_EXCEPTION(ConflictingFilenames, "Files list contains files with conflicting names");
DEFINE_EXCEPTION(FileAlreadyExists, "File already exists");

