#!/bin/bash
clear
echo -e "\n  Huffman Data Compression Tool  \n"
echo -e '  1.Compress'
echo -e '  2.Decompress'
echo -e '  3.Exit  '
echo ''
read choice

if [ $choice -eq 1 ]
    then
    echo -e '  Files in the current Directory are...'
    tree
    echo -e '\n  Enter the File Name to Be Compressed : '
    echo ''
    read fileName
    mkdir Data
    ./compress $fileName
    echo -e 'Done'
    fileName_hz="$fileName.hz"
    fileName_md=($(md5sum $fileName_hz))
    mv .~temp_hash_file $fileName_md
    mv $fileName_md ./Data/
fi

if [ $choice -eq 2 ]
    then
    echo -e '  Files in the current Directory are...'
    tree
    echo -e '\n  Enter the File Name to Be Compressed : '
    echo ''
    read fileNameHz
    fileNameHz_md=($(md5sum $fileNameHz))
    ./decompress $fileNameHz ./Data/$fileNameHz_md
    exit
fi

if [ $choice -eq 4 ]
    then
    exit
fi

exit
