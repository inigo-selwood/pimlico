# !/bin/bash -

# Make, silencing the output
make >/dev/null

# Check the making succeeded
if [[ $? -ne 0 ]]; then
    exit 1
fi

# Check the verbose flag
verbose=0
if [[ "$*" == *--verbose* ]]; then
    verbose=1
fi

scripts_passed=0
scripts_failed=0
scripts_thrown=0

printf "invalid scripts: \n"
for script in ./invalid/*.peg; do

    # Extract the name of the script, without its extension, and run it, piping
    # the output to two seperate file streams
    name="$(basename $script .peg)"
    ./test ${script} 1> ./output.txt 2> ./errors.txt
    return_value=$?

    # If the invalid script passed, that's an error -- report it
    if [[ $return_value -eq 0 ]]; then

        printf "! ${name} (failed)\n"
        ((scripts_failed=scripts_failed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./output.txt
        fi

    # If it exited with a non-zero error code, that's a pass
    elif [[ $return_value -eq 3 ]]; then

        printf "  ${name} (passed)\n";
        ((scripts_passed=scripts_passed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./errors.txt
        fi

    # If it threw an exception, that's an error
    elif [[ $return_value -eq 4 ]]; then

        printf "! ${name} (threw exception)\n"
        ((scripts_thrown=scripts_thrown+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./errors.txt
        fi
    fi
done

# Start printing valid test script results
printf "\n"
printf "valid scripts:\n"
for script in ./valid/*.peg; do

    # Extract the name of the script, without its extension, and run it, piping
    # the output to two seperate file streams
    name="$(basename $script .peg)"
    ./test ${script} 1> ./output.txt 2> ./errors.txt
    return_value=$?

    # If the script passes, log it
    if [[ $return_value -eq 0 ]]; then

        printf "  ${name} (passed)\n"
        ((scripts_passed=scripts_passed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./output.txt
        fi

    # Non-zero return codes are a fail
    elif [[ $return_value -eq 3 ]]; then

        printf "! ${name} (failed)\n";
        ((scripts_failed=scripts_failed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./errors.txt
        fi

    # As before, exceptions thrown also count as a failure
    elif [[ $return_value -eq 4 ]]; then

        printf "! ${name} (threw exception)\n"
        ((scripts_thrown=scripts_thrown+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./errors.txt
        fi
    fi
done

# Print the stats
printf "\n"
printf "summary:\n"
printf "  scripts passed:             ${scripts_passed}\n"
printf "  scripts failed:             ${scripts_failed}\n"
printf "  scripts threw an exception: ${scripts_thrown}\n"

# Do a bit of tidying up
rm -f ./output.txt ./errors.txt ./test
