
g++-7 ./tests/term/test.cpp -o ./tests/term/test -std=c++17 -I ./pimlico/

if [[ $? -ne 0 ]]; then
    printf "compilation failed\n"
    exit 1
fi

for file in ./tests/term/invalid/*.peg; do
    test_name="$(basename $file .peg)"

    ./tests/term/test $file

    if [[ $? -ne 0 ]]; then
        printf "  ${test_name} (passed)\n"
    else
        printf "! ${test_name} (failed)\n"
    fi

done

for file in ./tests/term/valid/*.peg; do
    test_name="$(basename $file .peg)"

    ./tests/term/test $file

    if [[ $? -ne 0 ]]; then
        printf "! ${test_name} (failed)\n"
    else
        printf "  ${test_name} (passed)\n"
    fi

done

rm ./tests/term/test
