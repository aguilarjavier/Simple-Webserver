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
    char buffer[BUFFER_SIZE + 1];
    pid_t childPid;

    struct sockaddr_in newaddr;
    socklen_t addr_size;

    // SET UP THE SOCK
    sock = socket(AF_INET, SOCK_STREAM, 0);

    memset(&servaddr, '\0', sizeof(servaddr));
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // BIND AND LISTEN
    bind(sock, (struct sockaddr *) &servaddr, sizeof(servaddr));

    listen(sock, 10);

    printf("Waiting for a TCP connection on port %d...\n", PORT);
        fflush(stdout);

    while (1)
    {
        connfd = accept(sock, (struct sockaddr *)&newaddr, &addr_size);
        if((childPid = fork()) == 0)
        {
            close(sock);

            while(1)
            {
                recv(connfd, buffer, BUFFER_SIZE, 0);

                if(strcmp(buffer, "exit") == 0)
                {
                    printf("Disconnected from client\n");
                    exit(1);
                }
                else 
                {
                    printf("Client: %s\n", buffer);
                    printf("Server: ");
                    scanf("%s",&buffer[0]);
                    send(connfd, buffer, strlen(buffer), 0);
                    bzero(buffer, sizeof(buffer));
                }

            }

        }

        
    }
    close(connfd);
            
    return 0;
    
}