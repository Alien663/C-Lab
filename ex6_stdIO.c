#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#define BLKSIZE 1024

char* replaceWord(const char* s, const char* oldW, const char* newW) { 
    char* result; 
    int i, cnt = 0; 
    int newWlen = strlen(newW); 
    int oldWlen = strlen(oldW); 
  
    // Counting the number of times old word 
    // occur in the string 
    for (i = 0; s[i] != '\0'; i++) { 
        if (strstr(&s[i], oldW) == &s[i]) { 
            cnt++; 
  
            // Jumping to index after the old word. 
            i += oldWlen - 1; 
        } 
    } 
  
    // Making new string of enough length 
    result = (char*)malloc(i + cnt * (newWlen - oldWlen) + 1); 
  
    i = 0; 
    while (*s) { 
        // compare the substring with the result 
        if (strstr(s, oldW) == s) { 
            strcpy(&result[i], newW); 
            i += newWlen; 
            s += oldWlen; 
        } 
        else
            result[i++] = *s++; 
    } 
  
    result[i] = '\0'; 
    return result; 
} 

void main(int argc, char *argv[]){
    // process stdin
    char *buff;
    if(argc<4) { 
        buff = (char *)malloc(1024 * sizeof(char));
        char *out_str;
        ssize_t size = read(STDIN_FILENO, buff, BLKSIZE);
        out_str = replaceWord(buff, argv[1], argv[2]);
        write(STDOUT_FILENO, out_str, strlen(out_str));

    }
    else{ // process argv
        for(int i=3; i<argc; i++){
            buff = (char *)malloc(1024 * sizeof(char));
            char *out_str;
            int fd = open(argv[i], O_RDONLY);
            ssize_t size = read(fd, buff, BLKSIZE);
            buff[size] = '\0';
            close(fd);
            out_str = replaceWord(buff, argv[1], argv[2]);
            write(STDOUT_FILENO, out_str, strlen(out_str));
        }
    }
}