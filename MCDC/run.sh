#!/bin/bash
echo "" > test_result
gcc -o tcas tcas.c
gcc -o random_generator random_generator.c
./random_generator
i=0
while read line || [ -n "$line" ] ; do
    echo "===============$i th input================" >> test_result
    echo "test input: $line" >> test_result
    ./tcas $line
    ((i+=1))
done < test_input
