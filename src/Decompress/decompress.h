#ifndef DECOMPRESS_H
#define DECOMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int lli;
#define BUFFER 256
#define TABLE_SIZE 3581

typedef struct data
{
    char code[BUFFER];
    char symbol;
}data;

typedef struct list
{
    data node;
    struct list* next;
}list;

void create_prime_table(int[],int);

void create_prime_table(int prime_table[], int rang)
{
    prime_table[0]=2;
    prime_table[1]=3;
    prime_table[2]=5;
    prime_table[3]=7;
    int i=4;
    int n,j;
    int prime;
    for(n=10; n<=rang; ++n)
    {
        prime=1;
        for(j=0; prime_table[j]*prime_table[j]<=n; ++j)
        {
            if(n%prime_table[j]==0)
            {
                prime=0;
                break;
            }
        }
        if(prime)
        {
            prime_table[i]=n;
            ++i;
        }
    }
}

void create_encode_map(list *encode_map[], FILE* fpe, int prime_table[])
{
    char line[256];
    int symbol;
    int count=0;
    lli key=0;
    unsigned i;
    while (fgets(line, sizeof(line), fpe))
    {
        line[strlen(line)-1] = '\0';
        if(count%2==0)
        {
            symbol = (int) strtol(line, (char **)NULL, 10);
        }
        else
        {
            key=0;
            for(i=0; i<strlen(line); ++i)
            {
                if(line[i]=='1')
                {
                    key += prime_table[i];
                }
            }

            list* l = (list*)malloc(sizeof(list));
            strcpy((l->node).code,line);
            (l->node).symbol = symbol;
            l->next = encode_map[key];
            encode_map[key] = l;
        }
        ++count;
    }
}

void print_encode_map(list *encode_map[])
{
    int i;
    for(i=0; i<TABLE_SIZE; ++i)
    {
        list *temp = encode_map[i];
        while(temp != NULL)
        {
            printf("%d %d %s\n",i,temp->node.symbol,(temp->node).code);
            temp=temp->next;
        }
    }
}

int encode_map_check(lli key,list *encode_map[],char code[])
{
    list *temp = encode_map[key];
    while(temp != NULL)
    {
        if((strcmp(code,temp->node.code))==0)
        {
            return (temp->node.symbol);
        }
        temp=temp->next;
    }
    return -1;
}

void decompress_file(FILE* fpz,FILE* fpw,list* encode_map[],int prime_table[])
{
    char c;
    unsigned char mask;
    int i=0,test;
    char test_code[BUFFER] = "";
    lli key=0;
    while ((c = fgetc(fpz)) != EOF)
    {
        mask = 0X80;
        while(mask)
        {
            if((mask&c)!=0)
            {
                strcat(test_code,"1");
                key+=prime_table[i];
            }
            else if((mask&c)==0)
            {
                strcat(test_code,"0");
            }
            ++i;
            test = encode_map_check(key,encode_map,test_code);
            if(test > -1)
            {
                fprintf(fpw,"%c",test);
                i=0;
                key=0;
                strcpy(test_code,"");
            }
            mask = mask>>1;
        }
    }
}

#endif
