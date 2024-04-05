cmake_minimum_required(VERSION 3.27)
project(gta3-img-builder)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_EXPORT_COMPILE_COMMANDS True)

include_directories(src)

add_library(img-builder-app
  src/FileListReader.cpp
  src/DirWriter.cpp
  src/ImgWriter.cpp
  src/ArchiveBuilder.cpp
  src/Application.cpp
  src/DirEntry.cpp
  src/ImgExtracter.cpp
)

include(FetchContent)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(tests
  test/TestUtils.cpp
  test/DirWriterTests.cpp
  test/ImgWriterTests.cpp
  test/ArchiveBuilderTests.cpp
  test/FileListReaderTests.cpp
  test/DirEntryTests.cpp
  test/ImgExtracterTests.cpp
)
target_link_libraries(tests GTest::gtest_main)
target_link_libraries(tests img-builder-app)

include(GoogleTest)
gtest_discover_tests(tests
  WORKING_DIRECTORY "${PROJECT_SOURCE_DIR}/test/assets/"
)

add_executable(gta3-img-builder src/main.cpp)
target_link_libraries(gta3-img-builder img-builder-app)

