#include <gtest/gtest.h>
#include "DirEntry.h"
#include "Exceptions.h"

bool operator==(const DirEntry& left, const DirEntry& right)
{
  return 0 == memcmp(&left, &right, sizeof(DirEntry));
}

TEST(ShouldThrowExceptionIfDirFileNotReadable, DirEntryTests)
{
  EXPECT_THROW({
    DirEntry::ParseDirFile("non_existing");
  }, CouldNotReadFile);
}

TEST(ShouldReadValidDirFileCorrectly, DirEntryTests)
{
  const auto result = DirEntry::ParseDirFile("valid1.dir");

  const std::vector<DirEntry> expected = {
    {
      .sectorStart = 3,
      .sectorCount = 5,
      .fileName = "derp"
    },
    {
      .sectorStart = 5,
      .sectorCount = 256,
      .fileName = "foo.bar"
    },
  };

  EXPECT_EQ(expected, result);
}

TEST(ShouldThrowExceptionIfDirFileSizeIsWrong, DirEntryTests)
{
  EXPECT_THROW({
    DirEntry::ParseDirFile("invalid1.dir");
  }, UnexpectedEof);
}

TEST(ShouldReadDirWithDuplicateNamesCorrectly, DirEntryTests)
{
  const auto result = DirEntry::ParseDirFile("valid2_duplicate_filenames.dir");

  const std::vector<DirEntry> expected = {
    {
      .sectorStart = 3,
      .sectorCount = 5,
      .fileName = "foo.bar"
    },
    {
      .sectorStart = 5,
      .sectorCount = 256,
      .fileName = "foo.bar"
    },
  };

  EXPECT_EQ(expected, result);
}
