#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

int bigEndianBytesToInt(unsigned char* b, unsigned length){
    int val = 0;
    unsigned i;
    for(i=0;i<length;++i){
        val = (val << 8) | (b[i] & 0xff);
    }
    return val;
}

void intToBigEndianBytes(int val, unsigned char* b, unsigned length){
    unsigned i;
    for(i=0;i<length;++i){
        b[length - 1 - i] = (unsigned char)(val & 0xff);
        val >>= 8;
    }
}

int comp(const void*a,const void*b)
{
    return *(int*)a-*(int*)b;
}

void toFile(int n, int *arr, char* Finename){
    int i, fp;
    unsigned char buff[4];
    fp = open(Finename, O_RDWR|O_CREAT, 0666);
    intToBigEndianBytes(n, buff, 4);
    write(fp, buff, 4);
    for(i=0;i<n;i++){
        intToBigEndianBytes(arr[i], buff, 4);
        write(fp, buff, 4);
    }
    close(fp);
}

int *fromFile(int* n, char* filename){
    int fp, i;
    int *ptr = NULL;
    unsigned char buff[4];
    fp = open(filename, O_RDONLY);
    if(fp < 0) return NULL;
    if(read(fp, buff, 4) != 4) {
        close(fp);
        return NULL;
    }
    *n = bigEndianBytesToInt(buff, 4);
    ptr = (int *)malloc(sizeof(int)*(*n));
    for(i=0;i<*n;i++){
        if(read(fp, buff, 4) != 4) {
            free(ptr);
            close(fp);
            return NULL;
        }
        ptr[i] = bigEndianBytesToInt(buff, 4);
    }
    close(fp);
    return ptr;
}

int main(int argc, char *argv[]){
    if(argc != 3){
        fprintf(stderr, "Usage: %s <input_file> <output_file>\n", argv[0]);
        return 1;
    }
    int n;
    int *ptr = fromFile(&n, argv[1]);
    if(ptr == NULL){
        fprintf(stderr, "Error reading input file\n");
        return 1;
    }
    qsort(ptr, n, sizeof(int), comp);
    toFile(n, ptr, argv[2]);
    free(ptr);
    return 0;
}