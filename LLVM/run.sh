#!/bin/bash

declare -a StringArray=("./test_argparse -f --path=/path/to/file a 2>&1" "./test_argparse -f -f --force --no-force 2>&1" "./test_argparse -i 2>&1" "./test_argparse -i 2 2>&1"  "./test_argparse -i2 2>&1" "./test_argparse -ia 2>&1" "./test_argparse -i 0xFFFFFFFFFFFFFFFFF 2>&1" "./test_argparse -s 2.4 2>&1" "./test_argparse -s2.4 2>&1" "./test_argparse -sa 2>&1" "./test_argparse -s 1e999 2>&1" "./test_argparse -f -- do -f -h 2>&1" "./test_argparse -tf 2>&1" "./test_argparse --read --write 2>&1" "./test_argparse -h" "./test_argparse --help" "./test_argparse --no-help 2>&1" )

idx=1
for i in "${StringArray[@]}";
do
        $i
        cat coverage_log >> TotalResult

        #idx=$(python -c "print $idx+1")
done
