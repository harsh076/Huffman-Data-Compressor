#!/bin/bash
rm -r compress main.o *.hz .~temp_hash_file ./Data/
clear
gcc -W -Wall -Wextra -c main.c -o main.o
if [ $? -eq 0 ]
    then
    gcc -o compress main.o
fi
exit
