#include <gtest/gtest.h>
#include "DirWriter.h"
#include "Exceptions.h"
#include "TestUtils.h"

TEST(ShouldThrowExceptionWhenWritingTooLongFilename, DirWriterTests)
{
  std::stringstream stream;
  DirWriter writer(stream);

  EXPECT_THROW({
    writer.Write(0, 1, "too_long_name_111111.ext");
  }, FileNameTooLong);
}

TEST(ShouldAddSingleEntryCorrectly, DirWriterTests)
{
  std::stringstream stream;
  DirWriter writer(stream);

  writer.Write(0xAABBCCDD, 0x12345678, "file.ext");

  const auto expectedBytes = HexStrToBytes(
    "DDCCBBAA"
    "78563412"
    "66696C652E65787400000000000000000000000000000000"
  );

  EXPECT_EQ(expectedBytes, GetWrittenBytes(stream));
}

TEST(ShouldAddMultipleEntriesCorrectly, DirWriterTests)
{
  std::stringstream stream;
  DirWriter writer(stream);

  writer.Write(0xAA, 0x31, "file1.ext");
  writer.Write(0xBB, 0x62, "file2.ext");

  const auto expectedBytes = HexStrToBytes(
    "AA000000"
    "31000000"
    "66696C65312E657874000000000000000000000000000000"
    "BB000000"
    "62000000"
    "66696C65322E657874000000000000000000000000000000"
  );

  EXPECT_EQ(expectedBytes, GetWrittenBytes(stream));
}

