
g++-7 ./tests/term/test.cpp -o ./tests/term/test -std=c++17 -I ./pimlico/

if [[ $? -ne 0 ]]; then
    printf "compilation failed\n"
    exit 1
fi

test_script=$1
if [[ ! -z $test_script ]]; then
    ./tests/term/test $test_script 2> errors.txt 1> output.txt

    return_value=$?
    test_name="$(basename $test_script .peg)"

    # Report parse failures
    if [[ $return_value -eq 1 ]]; then
        printf "! ${test_name} (failed)\n"

        while read line; do
            printf "    ${line}\n"
        done < errors.txt

    # Report exceptions
    elif [[ $return_value -eq 2 ]]; then
        printf "! ${test_name} (threw exception)\n"

        while read line; do
            printf "    ${line}\n"
        done < errors.txt

    # Print output
    else
        printf "  ${test_name} (passed)\n"

        while read line; do
            printf "    ${line}\n"
        done < output.txt
    fi

else
    for test_script in ./tests/term/invalid/*.peg; do
        ./tests/term/test $test_script 2> errors.txt 1> output.txt

        return_value=$?
        test_name="$(basename $test_script .peg)"

        # Report parse failures
        if [[ $return_value -eq 1 ]]; then
            printf "  ${test_name} (passed)\n"

        # Report exceptions
        elif [[ $return_value -eq 2 ]]; then
            printf "! ${test_name} (threw exception)\n"

            while read line; do
                printf "    ${line}\n"
            done < ./errors.txt

        # Print output
        else
            printf "! ${test_name} (failed)\n"
        fi
    done

    printf "\n";

    for test_script in ./tests/term/valid/*.peg; do
        ./tests/term/test $test_script 2> ./errors.txt 1> ./output.txt

        return_value=$?
        test_name="$(basename $test_script .peg)"

        # Report parse failures
        if [[ $return_value -eq 1 ]]; then
            printf "! ${test_name} (failed)\n"

            while read line; do
                printf "    ${line}\n"
            done < ./errors.txt

        # Report exceptions
        elif [[ $return_value -eq 2 ]]; then
            printf "! ${test_name} (threw exception)\n"

            while read line; do
                printf "    ${line}\n"
            done < ./errors.txt

        # Print output
        else
            printf "  ${test_name} (passed)\n"
        fi
    done
fi

rm -f ./errors.txt ./output.txt ./tests/term/test
