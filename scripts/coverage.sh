cmake .
if [[ $? -ne 0 ]]; then
    echo "cmake failed"
    exit 1
fi

make test --jobs=12
if [[ $? -ne 0 ]]; then
    echo "build failed"
    exit 1
fi

mkdir --parents coverage

./test/binaries/test

ROOT=`pwd`

gcovr \
    --root $ROOT \
    --filter $ROOT/source/ \
    --object-directory $ROOT/CMakeFiles/ \
    --delete \
    --output coverage/ \
    --html-details
