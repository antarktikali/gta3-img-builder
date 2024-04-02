#pragma once
#include <filesystem>
#include <string>

class Application
{
public:
  Application(
      const std::string& filesListToRead,
      const std::string& dirToWrite,
      const std::string& imgToWrite
  );

  void Run();
private:
  const std::filesystem::path filesListToRead;
  const std::filesystem::path dirToWrite;
  const std::filesystem::path imgToWrite;

};

