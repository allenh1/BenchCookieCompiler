#!/bin/bash

if [ "$#" -ne 1 ]; then
	echo "Invalid args. Need exactly 1!"
	exit 1
fi

test="t"$1".bc"
out="t"$1".out"
org="t"$1".org.out"

../../build/bcc $test > out/$out

if [ $? -ne 0 ]; then
	rm -f out/$out
	echo "Error running test!"
	exit 2
fi

diff=$(diff -u out/$out out/$org)
rm -f out/$out

if [ $diff -z ]; then
	exit 0
else	
	echo "Test $1 failed!"
	echo "diff:"
	echo $diff
	exit 3
fi
