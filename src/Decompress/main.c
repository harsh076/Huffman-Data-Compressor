#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "decompress.h"

int main(int argc, char *argv[])
{
    if(argc != 3)
    {
        printf("Error Occured !!!\n");
        return -1;
    }

    //Opening the compressed file
    FILE *fpz = NULL;
    char input_file[256] = "";
    //input_file is the name of the compressed file that is to be decompressed
    strcat(input_file,argv[1]);
    fpz = fopen(input_file,"r");
    if(fpz == NULL)
    {
        printf("\nError Occured !!!");
    }

    //Opening the File containing Encoding Information
    FILE *fpe = NULL;
    char encode_file[256] = "";
    //encode_file is the name of the file containing the encoding information
    strcat(encode_file,argv[2]);
    fpe = fopen(encode_file,"r");
    if(fpe == NULL)
    {
        printf("\nError Occured !!!");
    }

    int prime_table[5000] = {0};
    create_prime_table(prime_table,10000);

    //Create a Hash Table with key = f(code), and value = {symbol,code}

    list **encode_map = (list**) malloc(sizeof(list)*TABLE_SIZE);
    int i;
    for(i=0; i<TABLE_SIZE; ++i)
    {
        encode_map[i] = NULL;
    }
    create_encode_map(encode_map,fpe,prime_table);

    fclose(fpe);
    //print_encode_map(encode_map);
    //Crearing a New File to write the deconpressed output
    input_file[strlen(input_file)-3] = '\0';
    FILE* fpw = NULL;
    fpw = fopen(input_file,"w");
    if(fpw == NULL)
    {
        printf("\nError Occured !!!");
    }

    //Decompress the File.
    decompress_file(fpz,fpw,encode_map,prime_table);

    fclose(fpz);
    fclose(fpw);

    return 0;
}
