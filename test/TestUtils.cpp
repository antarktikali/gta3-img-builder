#include "TestUtils.h"

#include <fstream>

std::vector<uint8_t> HexStrToBytes(const std::string& hexStr)
{
  std::vector<uint8_t> bytes;

  for (size_t i = 0; i < hexStr.length(); i += 2) {
    const auto byteString = hexStr.substr(i, 2);
    const auto byte = strtol(byteString.c_str(), nullptr, 16);
    bytes.push_back(byte & 0xFF);
  }

  return bytes;
}

std::vector<uint8_t> GetWrittenBytes(const std::stringstream& stream)
{
  const std::string streamStr = stream.str();
  return {streamStr.begin(), streamStr.end()};
}

std::vector<uint8_t> GetWrittenBytes(const std::filesystem::path& path)
{
  std::ifstream file(path, std::ios::in | std::ios::binary);
  std::stringstream strStream;
  strStream << file.rdbuf();
  return GetWrittenBytes(strStream);
}

std::stringstream CreateStream(const std::string& hexStr)
{
  const auto bytes = HexStrToBytes(hexStr);

  std::stringstream stream(std::string(bytes.begin(), bytes.end()));
  return stream;
}

