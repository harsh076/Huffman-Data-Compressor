#ifndef COMPRESS_H
#define COMPRESS_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef long long int lli;
#define BUFFER 256

typedef struct node
{
    struct node* left;
    struct node* right;
    struct node* prnt;
    long double pi;
    char data;
    char code[260];
}node;

static int SHORTEST_CODE = 1;

int genrate_freq_table(lli*,FILE*);
int genrate_prob_table(long double*, lli* ,lli);

void make_p_queue(node *, long double*);
void print_p_queue(node*, int);

void create_min_heap(node *, int, int);
void min_heapify(node*, int, int, int);

node* genrate_tree(node*, int);
void print_tree(node *);

void genrate_codes(node*);
void genrate_code_table(char [][BUFFER],node*);
void print_code_table(char [][BUFFER]);

int genrate_compressed_file(char*, char[][BUFFER]);
void flush_buffer(char *, FILE*);

void write_hash_file(FILE*, char [][BUFFER]);

int genrate_freq_table(lli* index,FILE *fp)
{
    lli* freq_table = index + BUFFER/2;
    int i;
    for(i=0; i<BUFFER; ++i)
    {
        freq_table[i] = 0;
    }
    lli count = 0;
    char c;
    while((c = fgetc(fp)) != EOF)
    {
        ++freq_table[(int)c];
        ++count;
    }
    fclose(fp);
    return count;
}

int genrate_prob_table(long double *probability,lli *freq_table,lli count)
{
    int i;
    int act_size = 0;
    for(i=0; i<BUFFER; ++i)
    {
        if(freq_table[i] != 0)
        {
            probability[i] = freq_table[i]/(long double)count;
            ++act_size;
        }
        else
        {
            probability[i] = 9.9;
        }
    }
    return act_size;
}

void make_p_queue(node *p_queue, long double *probability)
{
    int id = 0;
    int i;
    for(i=0; i<BUFFER; ++i)
    {
        if(!( probability[i] > 1))
        {
            (p_queue + id)->pi = probability[i];
            (p_queue + id)->left = NULL;
            (p_queue + id)->right = NULL;
            (p_queue + id)->prnt = NULL;
            (p_queue + id)->data = (char)i;
            strcpy((p_queue + id)->code,"");
            ++id;
        }
    }
}

void print_p_queue(node* p_queue,int act_size)
{
    printf("Probabilities : \n\n");
    int i;
    for(i=0; i<act_size; ++i)
    {
        printf("%d\t%Lf\n",(p_queue+i)->data, (p_queue+i)->pi);
        printf("%s\n",(p_queue+i)->code);
    }
    printf("\n");
}

void create_min_heap(node *heap, int start, int end)
{
    int o = start-1;
    int n = end-start-1;
    int i;
    for(i = (n/2)+o; i>=start; i--)
    {
        min_heapify(heap,i,i,end);
    }
}

void min_heapify(node* heap, int i, int start, int end)
{
    int o = start-1;
    int l = 2*(i-o) + o;
    int r = l+1;
    int minimum = start;

    if ((l<end) && (((heap + l)->pi) < ((heap + i)->pi)))
    {
        minimum = l;
    }
    else
    {
        minimum = i;
    }
    if ((r<end) && (((heap + r)->pi) < ((heap + minimum)->pi)))
    {
        minimum = r;
    }
    if(minimum != i)
    {
        node temp = heap[i];
        heap[i] = heap[minimum];
        heap[minimum] = temp;
        min_heapify(heap,minimum,minimum,end);
    }
}

node* genrate_tree(node* p_queue,int act_size)
{
    int i;
    for(i=0; i<act_size-1; ++i)
    {
        node* zero = (node*)malloc(sizeof(node));
        node* one = (node*)malloc(sizeof(node));

        zero->pi = p_queue[i].pi;
        zero->data = p_queue[i].data;
        zero->left = p_queue[i].left;
        zero->right = p_queue[i].right;
        zero->prnt = p_queue[i].prnt;
        strcpy(zero->code,"");

        one->pi = p_queue[i+1].pi;
        one->data = p_queue[i+1].data;
        one->left = p_queue[i+1].left;
        one->right = p_queue[i+1].right;
        one->prnt = p_queue[i+1].prnt;
        strcpy(one->code,"");

        p_queue[i+1].pi = zero->pi + one->pi;
        p_queue[i+1].left = zero;
        p_queue[i+1].right = one;
        p_queue[i+1].prnt = NULL;
        zero->prnt = p_queue + i + 1;
        one->prnt = p_queue + i + 1;

        min_heapify(p_queue,i+1,i+1,act_size-1);
    }
    return (p_queue + act_size - 1);
}

void print_tree(node *root)
{
    if(root != NULL)
    {
        print_tree(root->left);
        if(root->left == NULL || root->right == NULL)
        {
            printf("%d\t%Lf\n",root->data,root->pi);
            printf("Code : %s\n\n",root->code);
        }
        print_tree(root->right);
    }
}

void genrate_codes(node *root)
{
    // If Nodes are not the Leaf Nodes
    if(root->left != NULL && root->right != NULL )
    {
        //To generate the Code for Left Child
        strcpy(root->left->code,root->code);
        strcat(root->left->code,"0");

        //To generate the Code for Right Child
        strcpy(root->right->code,root->code);
        strcat(root->right->code,"1");

        //Traverse the Subtree Recursively
        genrate_codes(root->left);
        genrate_codes(root->right);
    }
}

void genrate_code_table(char code_table[][BUFFER],node *root)
{
    if(root != NULL)
    {
        genrate_code_table(code_table,root->left);
        if(root->left == NULL || root->right == NULL)
        {
            strcpy(code_table[(int)root->data+128],root->code);
        }
        genrate_code_table(code_table,root->right);
    }
}

void print_code_table(char code_table[][BUFFER])
{
    printf("Code Table : \n\n");
    int i;
    int length = 0;
    int div = 0;
    for(i=0; i<BUFFER; ++i)
    {
        if(strcmp(code_table[i],"") != 0)
        {
            printf("%c\t%s\n",i,code_table[i]);
            length += strlen(code_table[i]);
            ++div;
        }
        if(strcmp(code_table[i],"0") == 0)
        {
            SHORTEST_CODE = 0;
        }
    }
    printf("Average Code length : %f\n",length/(double)div);
}

int genrate_compressed_file(char* input_file, char code_table[][BUFFER])
{
    FILE* fpr = NULL;
    fpr = fopen(input_file,"r");
    strcat(input_file,".hz");
    FILE* fpw = NULL;
    fpw = fopen(input_file,"wb");

    char stream[4*BUFFER] = "";
    char c;
    size_t top = 0;
    while((c = fgetc(fpr)) != EOF)
    {
        top += strlen(code_table[(int)c]);
        strcat(stream,code_table[(int)c]);
        while(top > BUFFER)
        {
            flush_buffer(stream,fpw);
            char* temp_stream;
            temp_stream = stream + BUFFER;
            strcpy(stream,temp_stream);
            top -= BUFFER;
        }
    }

    if(SHORTEST_CODE)
    {
        char temp_str[2*BUFFER];
        int i;
        for(i=0; i<BUFFER-1; ++i)
        {
            temp_str[i] = '1';
        }
        temp_str[i] = '\0';
        strcat(stream,temp_str);
        flush_buffer(stream,fpw);
    }
    else
    {
        char temp_str[2*BUFFER];
        int i;
        for(i=0; i<BUFFER-1; ++i)
        {
            temp_str[i] = '0';
        }
        temp_str[i] = '\0';
        strcat(stream,temp_str);
        flush_buffer(stream,fpw);
    }

    fclose(fpr);
    fclose(fpw);

    return 0;
}

void flush_buffer(char *stream, FILE* fpw)
{
    unsigned char compressed[BUFFER/8];
    int i,j;
    unsigned char c = 0;
    for(i=0; i<BUFFER/8; ++i)
    {
        c = 0;
        for(j=0; j<8; ++j)
        {
            c = c << 1;
            if(stream[i*8 + j] == '1')
            {
                c += 1;
            }
        }
        compressed[i] = c;
    }
    fwrite(compressed,sizeof(compressed),1,fpw);
}

void write_hash_file(FILE* fp,char code_table[][BUFFER])
{
    int i;
    for(i=0; i<BUFFER; ++i)
    {
        if(strcmp(code_table[i],"") != 0)
        {
            fprintf(fp,"%d\n%s\n",i,code_table[i]);
        }
    }
}

#endif
