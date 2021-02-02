# !/bin/bash -

make > /dev/null

if [[ $? -ne 0 ]]; then
    exit 1
fi

verbose=0
if [[ "$*" == *--verbose* ]]; then
    verbose=1
fi

scripts_passed=0
scripts_failed=0
scripts_thrown=0

printf "invalid scripts: \n"
for script in ./invalid/*.peg; do

    name="$(basename $script .peg)"
    ./test ${script} 1> ./output.txt 2> ./errors.txt
    return_value=$?

    if [[ $return_value -eq 0 ]]; then

        printf "! ${name} (failed)\n"
        ((scripts_failed=scripts_failed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./output.txt
        fi

    elif [[ $return_value -eq 3 ]]; then

        printf "  ${name} (passed)\n";
        ((scripts_passed=scripts_passed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./errors.txt
        fi

    elif [[ $return_value -eq 3 ]]; then

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

printf "\n"
printf "valid scripts:\n"
for script in ./valid/*.peg; do

    name="$(basename $script .peg)"
    ./test ${script} 1> ./output.txt 2> ./errors.txt
    return_value=$?

    if [[ $return_value -eq 0 ]]; then

        printf "  ${name} (passed)\n"
        ((scripts_passed=scripts_passed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./output.txt
        fi

    elif [[ $return_value -eq 3 ]]; then

        printf "! ${name} (failed)\n";
        ((scripts_failed=scripts_failed+1))

        if [[ $verbose -eq 1 ]]; then
            IFS=''
            while read -r data; do
                printf "    ${data}\n"
            done < ./errors.txt
        fi

    elif [[ $return_value -eq 3 ]]; then

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

printf "\n"
printf "summary:\n"
printf "  scripts passed:             ${scripts_passed}\n"
printf "  scripts failed:             ${scripts_failed}\n"
printf "  scripts threw an exception: ${scripts_thrown}\n"

rm -f ./output.txt ./errors.txt
