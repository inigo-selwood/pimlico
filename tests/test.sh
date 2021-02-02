

printf "testing terms\n"
(cd ./grammar/term && bash ../test.sh)

printf "\n"
printf "testing specifications\n"
(cd ./grammar/specification && bash ../test.sh)
