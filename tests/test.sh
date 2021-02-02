
verbose=0
if [[ "$*" == *--verbose* ]]; then
    verbose_flag="--verbose"
fi

printf "testing terms\n"
(cd ./grammar/term && bash ../test.sh ${verbose_flag})

printf "\n"
printf "testing specifications\n"
(cd ./grammar/specification && bash ../test.sh ${verbose_flag})
