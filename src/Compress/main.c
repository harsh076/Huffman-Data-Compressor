#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "compress.h"

int main(int argc, char *argv[])
{

    if(argc == 1)
    {
        printf("Error Occured !!!\n");
        printf("Usage : compress [file_name]\n");
        return -1;
    }
    FILE *fp = NULL;
    char input_file[256] = "";
    strcat(input_file,argv[1]);
    fp = fopen(input_file,"r");
    if(fp == NULL)
    {
        printf("\nError Occured !!!");
    }

    //Genrating the frecuency table to store the occurrences of different symbols.
    //size stores the total number of character ouccured or the total size of the file.
    lli freq_table[BUFFER];
    lli size = genrate_freq_table(freq_table,fp);

    //Calculating the probability Table frome freq_table and count.
    //act_size is the number of distint symbols that have ouccured.
    long double probability[BUFFER];
    int act_size = genrate_prob_table(probability,freq_table,size);

    //Creating the Heap with node containing
    node p_queue[act_size];
    make_p_queue(p_queue,probability);

    //Creating Min Heap/Min Priority Queue
    create_min_heap(p_queue,0,act_size);
    //print_p_queue(p_queue,act_size);

    //Genrating the Huffman Code Tree
    node* root = genrate_tree(p_queue,act_size);

    genrate_codes(root);
    //print_tree(root);

    //Creating Encode Table
    char code_table[BUFFER][BUFFER];
    int i;
    for(i=0; i<BUFFER; ++i)
    {
        strcpy(code_table[i],"");
    }
    genrate_code_table(code_table,root);
    //print_code_table(code_table);

    //Genrating the compressed file from the code_table
    int flag;
    flag = genrate_compressed_file(input_file,code_table);
    if(flag)
    printf("\nError Occured !!!");

    char temp_hash_file_name[256] = ".~temp_hash_file";
    fp = fopen(temp_hash_file_name,"w");
    if(fp == NULL)
    {
        printf("\nError Occured !!!");
    }

    //writes the encoding information to the file "~temp_hash_file.md5"
    write_hash_file(fp,code_table);

    return 0;
}
