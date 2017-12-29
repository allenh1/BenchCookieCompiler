#!/bin/bash

(( total=3 ))
(( tests=0 ))
(( i=1 ))

echo -e "\e[96mScanner Tests\e[0m"
while (( $i <= $total )); do
    ./test-one.sh $i
    if [[ "$?" -eq 0 ]]; then
        echo -e "\e[32mtest $i passed\e[0m"
        (( tests=$tests + 1 ))
    else
        echo -e "\e[31mtest $i failed\e[0m"
    fi
    (( i=$i + 1 ))
done

# set exit status
(( exit=($total - $tests) ))
exit $exit
