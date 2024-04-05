#pragma once
#include <cstdint>
#include <filesystem>
#include <sstream>
#include <string>
#include <vector>

std::vector<uint8_t> HexStrToBytes(const std::string&);
std::vector<uint8_t> GetWrittenBytes(const std::stringstream&);
std::vector<uint8_t> GetWrittenBytes(const std::filesystem::path&);
std::stringstream CreateStream(const std::string&);

