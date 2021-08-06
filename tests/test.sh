
# Check for the verbose flag
verbose=0
if [[ "$*" == *--verbose* ]]; then
    verbose_flag="--verbose"
fi

# Run term tests
printf "testing terms\n"
(cd ./grammar/term && bash ../test.sh ${verbose_flag})

# Run specification tests
printf "\n"
printf "testing specifications\n"
(cd ./grammar/specification && bash ../test.sh ${verbose_flag})
