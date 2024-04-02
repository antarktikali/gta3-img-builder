#include <gtest/gtest.h>
#include "ImgWriter.h"
#include "TestUtils.h"

TEST(ShouldFillSingleBlockWithFilling, ImgWriterTests)
{
  std::stringstream outStream;
  ImgWriter writer(outStream, 10);

  std::stringstream inStream;
  inStream << "hello";

  const size_t numBlocksWritten = writer.Write(inStream);
  EXPECT_EQ(1, numBlocksWritten);

  const auto expectedBytes = HexStrToBytes(
    "68656C6C6F"
    "0000000000"
  );

  EXPECT_EQ(expectedBytes, GetWrittenBytes(outStream));
}

TEST(ShouldWrite2BlocksWithoutFilling, ImgWriterTests)
{
  std::stringstream outStream;
  ImgWriter writer(outStream, 5);

  std::stringstream inStream;
  inStream << "hellohello";

  const size_t numBlocksWritten = writer.Write(inStream);
  EXPECT_EQ(2, numBlocksWritten);

  const auto expectedBytes = HexStrToBytes(
    "68656C6C6F"
    "68656C6C6F"
  );

  EXPECT_EQ(expectedBytes, GetWrittenBytes(outStream));
}

TEST(ShouldWrite3BlocksWithFilling, ImgWriterTests)
{
  std::stringstream outStream;
  ImgWriter writer(outStream, 5);

  std::stringstream inStream;
  inStream << "hellohellohe";

  const size_t numBlocksWritten = writer.Write(inStream);
  EXPECT_EQ(3, numBlocksWritten);

  const auto expectedBytes = HexStrToBytes(
    "68656C6C6F"
    "68656C6C6F"
    "6865000000"
  );

  EXPECT_EQ(expectedBytes, GetWrittenBytes(outStream));
}

