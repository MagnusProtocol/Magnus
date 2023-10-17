FILES="$(find libmagnus magnus tests | grep -E ".*(\.ino|\.cpp|\.c|\.h|\.hpp|\.hh)")"
clang-format --verbose -i -style=file $FILES
