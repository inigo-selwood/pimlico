cmake .

make test -j12

mkdir -p coverage

./test/binaries/test

gcovr \
    --root source/ \
    --object-directory CMakeFiles/ \
    --delete \
    --html-details coverage/ \
    -j12
