
g++-7 ./tests/grammar/term/test.cpp \
        -o ./tests/grammar/term/test \
        -I ./pimlico/ \
        -std=c++17

if [[ $? -ne 0 ]]; then
    printf "compilation failed\n"
    exit 1
fi

test_script=$1
if [[ ! -z $test_script ]]; then
    ./tests/grammar/term/test $test_script \
            2> ./tests/grammar/term/errors.txt \
            1> ./tests/grammar/term/output.txt

    return_value=$?
    test_name="$(basename $test_script .peg)"

    # Report parse failures
    if [[ $return_value -eq 1 ]]; then
        printf "! ${test_name} (failed)\n"

        while read line; do
            printf "    ${line}\n"
        done < ./tests/grammar/term/errors.txt

    # Report incomplete parses
    elif [[ $return_value -eq 2 ]]; then
        printf "! ${test_name} (failed)\n"

        while read line; do
            printf "    ${line}\n"
        done < ./tests/grammar/term/output.txt

    # Report exceptions
    elif [[ $return_value -eq 3 ]]; then
        printf "! ${test_name} (threw exception)\n"

        while read line; do
            printf "    ${line}\n"
        done < ./tests/grammar/term/errors.txt

    # Print output
    else
        printf "  ${test_name} (passed)\n"

        while read line; do
            printf "    ${line}\n"
        done < ./tests/grammar/term/output.txt
    fi

else
    for test_script in ./tests/grammar/term/invalid/*.peg; do
        ./tests/grammar/term/test $test_script \
                2> ./tests/grammar/term/errors.txt \
                1> ./tests/grammar/term/output.txt

        return_value=$?
        test_name="$(basename $test_script .peg)"

        # Report parse failures
        if [[ $return_value -eq 1 ]]; then
            printf "  ${test_name} (passed)\n"

            while read -r line; do
                printf "    ${line}\n"
            done < ./tests/grammar/term/errors.txt

        # Report incomplete parses
        elif [[ $return_value -eq 2 ]]; then
            printf "  ${test_name} (passed)\n"

            while read -r line; do
                printf "    ${line}\n"
            done < ./tests/grammar/term/errors.txt

        # Report exceptions
        elif [[ $return_value -eq 3 ]]; then
            printf "! ${test_name} (threw exception)\n"

            while read line; do
                printf "    ${line}\n"
            done < ./tests/grammar/term/errors.txt

        # Print output
        else
            printf "! ${test_name} (failed)\n"
        fi
    done

    printf "\n";

    for test_script in ./tests/grammar/term/valid/*.peg; do
        ./tests/grammar/term/test $test_script \
                2> ./tests/grammar/term/errors.txt \
                1> ./tests/grammar/term/output.txt

        return_value=$?
        test_name="$(basename $test_script .peg)"

        # Report parse failures
        if [[ $return_value -eq 1 ]]; then
            printf "! ${test_name} (failed)\n"

            while read line; do
                printf "    ${line}\n"
            done < ./tests/grammar/term/errors.txt

        # Report parse failures
        elif [[ $return_value -eq 2 ]]; then
            printf "! ${test_name} (failed)\n"

            while read line; do
                printf "    ${line}\n"
            done < ./tests/grammar/term/errors.txt

        # Report exceptions
        elif [[ $return_value -eq 3 ]]; then
            printf "! ${test_name} (threw exception)\n"

            while read line; do
                printf "    ${line}\n"
            done < ./tests/grammar/term/errors.txt

        # Print output
        else
            printf "  ${test_name} (passed)\n"

            # while read -r line; do
            #     printf "    ${line}\n"
            # done < ./tests/grammar/term/output.txt

            printf "    ";
            cat ./tests/grammar/term/output.txt
        fi
    done
fi

rm -f ./tests/grammar/term/errors.txt \
        ./tests/grammar/term/output.txt \
        ./tests/grammar/term/test
