#include "Application.h"
#include <iostream>
#include <string>

void PrintHelp(const std::string_view& exeName)
{
  std::cout << "Usage: " << exeName <<
            " <files_list_file> <dir_file> <img_file>" << std::endl;

  std::cout <<
  std::endl << "  <files_list_file>: Text file containing paths to the files to add to the archive." <<
  std::endl << "  <dir_file>:        Path of the new .dir file to write." <<
  std::endl << "  <img_file>:        Path of the new .img file to write." <<
  std::endl;
}

bool ContainsHelpRequest(int argc, char* argv[])
{
  for (size_t i = 1; i < argc; i++) {
    const std::string parameter = argv[i];
    if (0 == parameter.compare("--help") || 0 == parameter.compare("-h")) {
      return true;
    }
  }
  return false;
}

int main(int argc, char* argv[])
{
  if (ContainsHelpRequest(argc, argv)) {
    PrintHelp(argv[0]);
    return 0;
  } else if (argc < 4) {
    PrintHelp(argv[0]);
    return 1;
  }

  const auto files_list_file = argv[1];
  const auto dir_file = argv[2];
  const auto img_file = argv[3];

  try {
    Application app(files_list_file, dir_file, img_file);
    app.Run();
  } catch (const std::exception& e) {
    std::cerr << "Error: " << e.what() << std::endl;
    return 1;
  }
  return 0;
}

