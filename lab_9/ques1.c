#include <stdio.h>
#include <stdlib.h>
#include <sys/select.h>
#include <time.h>

#define STDIN 0

int main(){
    fd_set readfd;
    char ch;
    struct timeval tv;
    tv.tv_sec = 500000;
    FD_ZERO(&readfd);
    FD_SET(STDIN,&readfd);
    
    int x = select(FD_SETSIZE,&readfd,NULL,NULL,&tv);
    if(x==0)
        printf("No Data");
    else if(x == -1)
        printf("Error");
    else{
        if(FD_ISSET(STDIN,&readfd)){
            scanf("%c",&ch);
            printf("%c",ch);
        }
    }
    return 0;
}

// what does this code do?

// It waits for 500000 seconds for input from stdin. If there is no input, it prints "No Data". 
// If there is input, it prints the input.
