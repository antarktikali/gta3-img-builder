#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include "ImgExtracter.h"
#include "Exceptions.h"
#include "TestUtils.h"

class ImgExtracterTests : public testing::Test
{
  protected:
    void SetUp() override
    {
      tmpDir = std::tmpnam(nullptr);
      EXPECT_FALSE(std::filesystem::exists(tmpDir));
      std::filesystem::create_directory(tmpDir);
    }

    void TearDown() override
    {
      if (!tmpDir.empty())
      {
        std::cout << "Cleaning up temporary test directory: " << tmpDir << std::endl;
        std::filesystem::remove_all(tmpDir);
      }
    }

    std::filesystem::path GetTmpDir() const
    {
      return tmpDir;
    }

  private:
    std::filesystem::path tmpDir;
};

TEST_F(ImgExtracterTests, ShouldThrowExceptionIfOutputDirectoryIsAFile)
{
  std::stringstream stream = CreateStream("");
  std::filesystem::path outDir = GetTmpDir() / "new_file";
  std::ofstream newFile(outDir);
  newFile << "\0";
  EXPECT_TRUE(std::filesystem::exists(outDir));

  EXPECT_THROW({
    ImgExtracter(1, {}, stream, outDir);
  }, OutputPathNotEmptyDirectory);
}

TEST_F(ImgExtracterTests, ShouldThrowExceptionIfOutputDirectoryIsNotEmpty)
{
  std::stringstream stream = CreateStream("");
  std::filesystem::create_directory(GetTmpDir() / "new_dir");

  EXPECT_THROW({
    ImgExtracter(1, {}, stream, GetTmpDir());
  }, OutputPathNotEmptyDirectory);
}

TEST_F(ImgExtracterTests, ShouldWriteNewFilesCorrectly)
{
  const std::vector<DirEntry> entries = {
    {
      .sectorStart = 0,
      .sectorCount = 1,
      .fileName = "test1.file"
    },
    {
      .sectorStart = 2,
      .sectorCount = 2,
      .fileName = "test2.file"
    }
  };

  std::stringstream stream = CreateStream("AABBCCDD11223344FFFF");

  ImgExtracter(2, entries, stream, GetTmpDir()).Extract();

  EXPECT_TRUE(std::filesystem::exists(GetTmpDir() / "test1.file"));
  EXPECT_EQ(HexStrToBytes("AABB"), GetWrittenBytes(GetTmpDir() / "test1.file"));

  EXPECT_TRUE(std::filesystem::exists(GetTmpDir() / "test2.file"));
  EXPECT_EQ(HexStrToBytes("11223344"), GetWrittenBytes(GetTmpDir() / "test2.file"));
}

TEST_F(ImgExtracterTests, ShouldCreateNecessaryDirectoriesWhenCreatingFiles)
{
  const std::vector<DirEntry> entries = {
    {
      .sectorStart = 0,
      .sectorCount = 1,
      .fileName = "test.file"
    }
  };

  std::stringstream stream = CreateStream("AA");

  const std::filesystem::path outPath = GetTmpDir() / "somedir" / "otherdir";
  ImgExtracter(1, entries, stream, outPath).Extract();

  EXPECT_TRUE(std::filesystem::exists(outPath / "test.file"));
}

TEST_F(ImgExtracterTests, ShouldCreateNewDirectoriesForDuplicateFiles)
{
  const std::vector<DirEntry> entries = {
    {
      .sectorStart = 0,
      .sectorCount = 1,
      .fileName = "test.file"
    },
    {
      .sectorStart = 1,
      .sectorCount = 1,
      .fileName = "test.file"
    },
    {
      .sectorStart = 2,
      .sectorCount = 1,
      .fileName = "test.file"
    }
  };

  std::stringstream stream = CreateStream("AABBCC");

  const std::filesystem::path outPath = GetTmpDir();
  ImgExtracter(1, entries, stream, outPath).Extract();

  EXPECT_TRUE(std::filesystem::exists(outPath / "test.file"));
  EXPECT_EQ(HexStrToBytes("AA"), GetWrittenBytes(outPath / "test.file"));

  EXPECT_TRUE(std::filesystem::exists(outPath / "1" / "test.file"));
  EXPECT_EQ(HexStrToBytes("BB"), GetWrittenBytes(outPath / "1" / "test.file"));

  EXPECT_TRUE(std::filesystem::exists(outPath / "2" / "test.file"));
  EXPECT_EQ(HexStrToBytes("CC"), GetWrittenBytes(outPath / "2" /"test.file"));
}

TEST_F(ImgExtracterTests, ShouldWriteNewFileCorrectly)
{
  const std::vector<DirEntry> entries = {
    {
      .sectorStart = 0,
      .sectorCount = 1,
      .fileName = "test.file"
    }
  };

  std::stringstream stream = CreateStream("AABBCCDD");
  const std::vector<uint8_t> expectedBytes = {0xAA, 0xBB};

  ImgExtracter(2, entries, stream, GetTmpDir()).Extract();

  EXPECT_TRUE(std::filesystem::exists(GetTmpDir() / "test.file"));
  EXPECT_EQ(expectedBytes, GetWrittenBytes(GetTmpDir() / "test.file"));
}

TEST_F(ImgExtracterTests, ShouldThrowExceptionIfDirEntryDoesNotMatchWithImg)
{
  const std::vector<DirEntry> entries = {
    {
      .sectorStart = 3,
      .sectorCount = 1,
      .fileName = "file.test"
    }
  };

  std::stringstream stream = CreateStream("AABB");

  EXPECT_THROW({
    ImgExtracter(1, entries, stream, GetTmpDir()).Extract();
  }, CouldNotReadSector);
}

