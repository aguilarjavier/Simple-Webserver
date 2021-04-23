#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>             // for memset()
#include <sys/socket.h>         // socket defs
#include <netdb.h>              // htonl/s, sockaddr_in  
#include <unistd.h>             // for read()

#define BUFFER_SIZE  2048
#define PORT  50000


int main(int argc, char** argv)
{
    int sock, connfd, n;
    struct sockaddr_in servaddr;
    uint8_t buffer[BUFFER_SIZE + 1];
    uint8_t recvline[BUFFER_SIZE + 1];

    // SET UP THE SOCK
    sock = socket(AF_INET, SOCK_STREAM, 0);

    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // BIND AND LISTEN
    bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(sock, 10);

    while (1)
    {
        struct sockaddr_in addr;
        socklen_t addr_len;

        printf("Waiting for a TCP connection on port %d...", PORT);
        fflush(stdout);

        connfd = accept(sock, (struct sockaddr *)NULL, NULL);

        memset(recvline, 0, BUFFER_SIZE);


        while( (n = read(connfd, recvline, BUFFER_SIZE - 1) ) > 0)
        {
            fprintf(stdout, "\n%s\n", recvline);

            if (recvline[n - 1] == '\n')
                break;
        }

        if (n < 0)
            printf("Error after reading\n");


        // send response
        snprintf((char*)buffer, sizeof(buffer), "HTTP/1.0 200 OK\r\n\r\nHello World\n");

        write(connfd, (char*)buffer, strlen((char*)buffer));
        close(connfd);



    }
    
    

    return 0;
}