# Intro
This tool can be used to modify .img files for GTA3 and GTA-VC. More
information about the .img format can be found
[here](https://gtamods.com/wiki/IMG_archive).

Only tested on GNU/Linux with [reVC](https://github.com/halpz/re3).

# Building
Cmake is used for building and running the tests. The test use gtest, which is
automatically downloaded by cmake.

```
mkdir build
cd build
cmake ../
make
```

Note: Implementation only uses STL, so no 3rd party dependencies are needed.
Should be straightforward to build without cmake if wanted.

## Tests
There are some tests that read some dummy files. Tests can be ran with the
`ctest` command so that the testing binary can find those files.

```
mkdir build
cd build
cmake ../
make
ctest
```

# Usage
The program supports extracting and building of .img archives. See the `--help`
command for more details. The paths are parsed with
[std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path), so
the format is platform dependent.

## Extracting an existing archive
```
./gta3-img-builder extract gta3.dir gta3.img output_directory
```

Extracts the given archive and writtes the files to the given output directory.
Also prints the path to the created files to the stdout. These paths can be
captured to create a files_list.txt which can be used when building a new
archive so that the order of files would be the same as in the original
archive.

```
./gta3-img-builder extract gta3.dir gta3.img output_directory > files_list.txt
```

## Building a new archive
```
./gta3-img-builder build new.dir new.img files_list.txt
```

The program takes a text file as a parameter. This file should include paths to
all the files that are to be added to the .img archive.  The files are added
with the same order as in the given list. The order of the files might matter
on older hardware, for example if the game is going to load multiple files with
one call.

