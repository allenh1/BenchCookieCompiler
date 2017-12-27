#!/usr/bin/env bash

echo -e "\e[96mLinter Test\e[0m"
# Use Linux kernel style
config=test/lint/linux.cfg
(( errors=0 ))

# make sure we are in the right place
if [[ ! -f LICENSE ]]; then
    echo - "\e[31mRun linter script from the root of the repo.\e[0m"
    exit 1
fi

# Lint the sources.
for c_file in src/*.c; do
    uncrustify -c $config $c_file
    dif=$(diff -ur $c_file $c_file.uncrustify)

    if [[ ! -z $dif ]]; then
        echo -e "\[31m$c_file does not conform to Kernel style!"
        echo "Patch will be found in $c_file.patch."
        diff -ur $c_file $c_file.uncrustify > $c_file.patch
        rm $c_file.uncrustify
        echo "You may apply the patch like this:"
        echo -e "    patch -p1 $c_file < $c_file.patch\e[0m"
        exit 2
    fi
    rm $c_file.uncrustify
done

echo -e "\e[32mLinter test passed\e[0m"
exit 0
