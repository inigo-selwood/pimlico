cmake .

make test -j12

mkdir -p coverage

./test/binaries/test
