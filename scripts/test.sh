cmake .
if [[ $? -ne 0 ]]; then
    echo "cmake failed"
    exit 1
fi

make test -j12
if [[ $? -ne 0 ]]; then
    echo "build failed"
    exit 1
fi

mkdir --parents coverage

./test/binaries/test
