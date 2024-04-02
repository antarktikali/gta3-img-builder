# Intro
This tool can be used to create .img files for GTA3 and GTA-VC. More
information about the .img format can be found
[here](https://gtamods.com/wiki/IMG_archive).

Only tested on GNU/Linux with [reVC](https://github.com/halpz/re3).

# Building
Only uses STL, so no 3rd party dependencies are needed.

```
mkdir build
cd build
cmake ../
make
```

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
The program takes a text file as a parameter. This file should include paths to
all the files that are to be added to the .img archive. The paths are parsed
with
[std::filesystem::path](https://en.cppreference.com/w/cpp/filesystem/path), so
the format is platform dependent. The files are added with the same order as in
the given list. The order of the files might matter on older hardware, for
example if the game is going to load multiple files with one call.

The other parameters are the paths to the new .img and .dir files to write. See
the `--help` command for more details.

