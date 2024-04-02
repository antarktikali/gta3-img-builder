#pragma once
#include <cstdint>
#include <sstream>
#include <string>
#include <vector>

std::vector<uint8_t> HexStrToBytes(const std::string&);
std::vector<uint8_t> GetWrittenBytes(const std::stringstream&);

