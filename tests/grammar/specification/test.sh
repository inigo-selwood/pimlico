g++-7 ./tests/grammar/specification/test.cpp \
        -o ./tests/grammar/specification/test \
        -I ./pimlico/ \
        -std=c++17

if [[ $? -ne 0 ]]; then
    printf "compilation failed\n"
    exit 1
fi

test_script=$1
if [[ ! -z $test_script ]]; then
    ./tests/grammar/specification/test $test_script \
            2> ./tests/grammar/specification/errors.txt \
            1> ./tests/grammar/specification/output.txt

    return_value=$?
    test_name="$(basename $test_script .peg)"

    # Report parse failures
    if [[ $return_value -eq 1 ]]; then
        printf "! ${test_name} (failed)\n"

        while read -r line; do
            printf "    ${line}\n"
        done < ./tests/grammar/specification/errors.txt

    # Report incomplete parses
    elif [[ $return_value -eq 2 ]]; then
        printf "! ${test_name} (threw exception)\n"

        while read line; do
            printf "    ${line}\n"
        done < ./tests/grammar/specification/errors.txt

    # Print output
    else
        printf "  ${test_name} (passed)\n"

        while read -r line; do
            printf "    ${line}\n"
        done < ./tests/grammar/specification/output.txt
    fi
else
    printf "no grammar file specified\n"
    exit 1
fi

rm -f ./tests/grammar/specification/errors.txt \
        ./tests/grammar/specification/output.txt \
        ./tests/grammar/specification/test
