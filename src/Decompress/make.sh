#!/bin/bash
rm -r decompress main.o
clear
gcc -W -Wall -Wextra -c main.c -o main.o
if [ $? -eq 0 ]
    then
    gcc -o decompress main.o
fi
exit
