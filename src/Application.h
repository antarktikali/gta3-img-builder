#pragma once
#include <string>
#include <vector>

class Application
{
public:
  Application(int, char**);
  int Run();
private:
  enum class Command
  {
    INVALID,
    BASIC_HELP,
    BUILD,
    EXTRACT
  };

  const static size_t SECTOR_SIZE = 2048;

  static std::vector<std::string> GetArguments(int, char**);
  static bool IsHelpArgument(const std::string&);

  void PrintBasicHelp() const;
  void PrintBuildHelp() const;
  void PrintExtractHelp() const;

  bool ContainsHelpArgument() const;
  int DoBuild() const;
  int DoExtract() const;
  Command GetCommand() const;

  const std::string exeName;
  const std::vector<std::string> arguments;
};

