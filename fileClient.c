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
   int clientSock, ret;
   struct sockaddr_in servAddr;
   char buffer[BUFFER_SIZE + 1];

   clientSock = socket(AF_INET, SOCK_STREAM, 0);
   if(clientSock < 0)
   {
       printf("ERROR in socket()\n");
       exit(1);
   }

   memset(&servAddr, '\0', sizeof(servAddr));
   servAddr.sin_family = AF_INET;
   servAddr.sin_port = htons(PORT);
   servAddr.sin_addr.s_addr = htonl(INADDR_ANY);

   ret = connect(clientSock, (struct sockaddr*)&servAddr, sizeof(servAddr));
   if(ret < 0)
   {
       printf("ERROR in connect()\n");
       exit(1);
   }

   while(1)
   {
       printf("Client request: ");
       scanf("%s", &buffer[0]);
       //printf("Requesting %s from server\n", buffer);
       send(clientSock, buffer, strlen(buffer), 0);
       bzero(buffer, sizeof(buffer));

       if(strcmp(buffer, "exit") == 0)
       {
           close(clientSock);
           printf("Goodbye!\n");
           exit(1);
       }

       printf("Server: ");
       
        if(recv(clientSock, buffer, BUFFER_SIZE, 0) < 0)
        {
            printf("ERROR in recv()\n");
            
        }
       else if (strcmp(buffer, "ERROR") == 0)
       {
            printf("ERROR\n\n");
            continue;
       }
       else
       {
           printf("\n%s\n\n",buffer );
       }

   }

    
    

    return 0;
}