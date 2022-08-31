FILES="$(find libmagnus magnus | grep -E ".*(\.ino|\.cpp|\.c|\.h|\.hpp|\.hh)")"
clang-format --verbose -i -style=file $FILES
