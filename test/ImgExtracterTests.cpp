#include <filesystem>
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

TEST_F(ImgExtracterTests, ShouldThrowExceptionIfFileAlredyExistsInOutDir)
{
  const std::vector<DirEntry> entries = {
    {
      .sectorStart = 0,
      .sectorCount = 1,
      .fileName = "file3.test"
    }
  };

  std::stringstream stream = CreateStream("AABBCCDD");

  EXPECT_THROW({
    ImgExtracter(1, entries, stream, "folder1").Extract();
  }, FileAlreadyExists);
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

