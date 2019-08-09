#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char **argv){
    unsigned char test[] = "░";
    unsigned char test2[] = "▒";
    unsigned char test3[] = "▓";
    unsigned char test4[] = "█";
    unsigned char test5[] = " ";
    for(int i=0; test2[i]; i++){
        for (int j = 0; j < 8; j++) {
            printf("%d", !!((test[i] << j) & 0x80));
        }
        printf("\n");
    }

    printf("%lu\n",strlen(test));
    printf("%lu\n",strlen(test2));
    printf("%lu\n",strlen(test3));
    printf("%lu\n",strlen(test4));
    printf("%lu\n",strlen(test5));
    return 0;
}
