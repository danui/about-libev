#!/bin/bash

for i in $@; do
    echo "______________________________________________________________________"
    echo "$i"
    echo
    $i
    exit_code=$?
    if [ $exit_code -ne 0 ]; then
	echo
	echo "$i had an error, exit code is ${exit_code}"
	exit 1
    fi
    echo
done