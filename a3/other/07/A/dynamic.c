#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
learned from this file:

Invalid write of size 1
at 0x4C34E00: strcpy (in /usr/lib/valgrind/vgpreload_memcheck-amd64-linux.so

- gcc -g -O0  dynamic.c  -o  dynamic
- valgrind --leak-check=yes ./dynamic "this is the time for all good people to aid their county" 2&> temp.txt
 
*/

// *** 50 
#define MAX_LEN 100

int main(int argc, char *argv[]) {
    char *line = NULL;
    char *t;
    int  num = 0;

    if (argc < 2) {
        fprintf(stderr, "usage: %s <some string>\n", argv[0]);
        exit(1);
    }
    
    // here is the magic line
    line = (char *)malloc(sizeof(char) * MAX_LEN);
    if (line == NULL) {
        fprintf(stderr,
            "Argh. Something bad happened with malloc. :-(\n");
    }

    strncpy(line, argv[1], MAX_LEN);

    t = strtok(line, " ");
    while (t) {
        num++;
        printf("Word: %s\n", t);
        t = strtok(NULL, " ");
    }
  
    printf("Number of words: %d\n", num);
    free(line);
    exit(0); 
}
