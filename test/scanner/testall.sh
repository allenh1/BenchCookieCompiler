#!/bin/bash

((total=0))
((tests=1))
((i=0))

while [[ "$i" -ne "$tests" ]]; do
	if [[ "$?" -eq 0 ]]; then
		((total=$total + 1))
	fi

	((i=$i + 1))
done

((exit=($tests - $total)))
exit $exit
