#!/usr/bin/env bash

# Use Linux kernel style
config=test/lint/linux.cfg
(( errors=0 ))

# make sure we are in the right place
if [[ ! -f LICENSE ]]; then
    echo "Run linter script from the root of the repo."
    exit 1
fi

# Lint the sources.
for c_file in src/*.c; do
    uncrustify -c $config $c_file
    dif=$(diff -ur $c_file $c_file.uncrustify)

    if [[ ! -z $dif ]]; then
        echo "src/$c_file does not conform to Kernel style!"
        echo "Patch will be found in $c_file.patch."
        diff -ur $c_file $c_file.uncrustify > $c_file.patch
        rm $c_file.uncrustify
        exit 2
    fi
    rm $c_file.uncrustify
done
