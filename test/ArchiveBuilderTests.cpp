#include <gtest/gtest.h>
#include "ArchiveBuilder.h"
#include "TestUtils.h"
#include "Exceptions.h"

TEST(TestBuildingArchiveWith3Files, ArchiveBuilderTests)
{
  std::stringstream dirOut, imgOut;
  DirWriter dirWriter(dirOut);
  ImgWriter imgWriter(imgOut, 3);
  ArchiveBuilder builder(dirWriter, imgWriter);
  const auto expectedDir = HexStrToBytes(
    "00000000"
    "02000000"
    "66696C65322E746573740000000000000000000000000000"
    "02000000"
    "01000000"
    "66696C65312E746573740000000000000000000000000000"
    "03000000"
    "02000000"
    "66696C65332E746573740000000000000000000000000000"
  );
  const auto expectedImg = HexStrToBytes(
    "AABBCCDD0000"
    "102030"
    "CAFECAFE0000"
  );

  builder.AddFile("file2.test");
  builder.AddFile("file1.test");
  builder.AddFile("folder1/file3.test");

  EXPECT_EQ(expectedImg, GetWrittenBytes(imgOut));
  EXPECT_EQ(expectedDir, GetWrittenBytes(dirOut));
}

TEST(BuildingArchiveWithTooLongNameShouldFail, ArchiveBuilderTests)
{
  std::stringstream dirOut, imgOut;
  DirWriter dirWriter(dirOut);
  ImgWriter imgWriter(imgOut, 10);
  ArchiveBuilder builder(dirWriter, imgWriter);

  EXPECT_THROW({
    builder.AddFile("too_long_file_name1.test");
  }, FileNameTooLong);
}

TEST(BuildingArchiveNonExistingFileShouldFail, ArchiveBuilderTests)
{
  std::stringstream dirOut, imgOut;
  DirWriter dirWriter(dirOut);
  ImgWriter imgWriter(imgOut, 10);
  ArchiveBuilder builder(dirWriter, imgWriter);

  EXPECT_THROW({
    builder.AddFile("non_existing");
  }, CouldNotReadFile);
}

