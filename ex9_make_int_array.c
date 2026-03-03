#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>

void intToBigEndianBytes(int val, unsigned char* b, unsigned length){
    unsigned i;
    for(i=0;i<length;++i){
        b[length - 1 - i] = (unsigned char)(val & 0xff);
        val >>= 8;
    }
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

int main(int argc, char *argv[]){
    int i, n;
    int *ptr = NULL;
    n = argc - 2;
    ptr = (int *)malloc(sizeof(int)*n);
    for(i=2;i<argc;i++)
        ptr[i-2] = atoi(argv[i]);
    toFile(n, ptr, argv[1]);
    free(ptr);
    return 0;
}
