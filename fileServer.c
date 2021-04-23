#include <stdio.h>
#include <stdlib.h>
#include <stdint.h> 
#include <string.h>             // for memset()
#include <sys/socket.h>         // socket defs
#include <netdb.h>              // htonl/s, sockaddr_in  
#include <unistd.h>             // for read()
#include <limits.h>



#define BUFFER_SIZE  2048
#define PORT  50000


int main(int argc, char** argv)
{
    int serverSocket, clientSocket, addrSize;
    struct sockaddr_in servAddr, clientAddr;

    char buffer[BUFFER_SIZE + 1];
    size_t bytes_read;
    int msgsize;
    char path[PATH_MAX + 1];

    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    if( serverSocket < 0)
    {
        printf("ERROR in socket()\n");
        exit(1);
    }

    servAddr.sin_family = AF_INET;
    servAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servAddr.sin_port = htons(PORT);

    bind(serverSocket, (struct sockaddr *) &servAddr, sizeof(servAddr));

    listen(serverSocket, 10);

    while(1)
    {
       

        printf("Waiting for a TCP connection on port %d...\n", PORT);
        addrSize = sizeof(struct sockaddr_in);
        
        clientSocket = accept(serverSocket, (struct sockaddr*) &clientAddr, (socklen_t*)&addrSize);

        while((bytes_read = read(clientSocket, buffer + msgsize, sizeof(buffer) - msgsize)) > 0)
        {
            msgsize += bytes_read;
            if(msgsize > BUFFER_SIZE - 1 || buffer[msgsize - 1] == '\n')
                break;

            //buffer[msgsize - 1] = 0;

            printf("Request: %s\n", buffer);
            

            FILE* fp = fopen(buffer, "r");
            if(fp == NULL)
            {
                printf("ERROR in fopen()\n\n");
                write(clientSocket, "ERROR", strlen("ERROR"));
                bzero(buffer, sizeof(buffer));
                msgsize = 0;
                continue;
                // close(clientSocket);
                // exit(1);
            }

            else 
            {
                while ((bytes_read = fread(buffer, 1, BUFFER_SIZE, fp)) > 0)
                {
                    printf("Sending file\n\n");
                    write(clientSocket, buffer, bytes_read);
                }
            }
            bzero(buffer, sizeof(buffer));
            msgsize = 0;
            //close(clientSocket);
            fclose(fp);
            //printf("Disconnected\n");
        }


        



    }

    return 0;
}