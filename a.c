#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct subject {
    char* name;
    int length;
    struct subject* next;
};

int main() {
    char* b="klsjdfashfgkjhaslgjha";
    char *a=(char*)(malloc(10*sizeof(char)));
    strncpy(a,b,5);
    
    printf("%s",a);

    return 0;
}
