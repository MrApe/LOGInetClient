#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <stdint.h>
#include <errno.h>
 
#define MAXRCVLEN 1000
#define PORTNUM 2300



int main(int argc, char *argv[])
{
   char rcv_buf[MAXRCVLEN + 1]; /* +1 so we can add null terminator */
   int nbytes, in_socket_fd;
   struct sockaddr_in dest; 
 
   in_socket_fd = socket(AF_INET, SOCK_STREAM, 0);
  
   if (argc < 2)
   {
      printf("\nToo few arguments. Usage: ./LogINETClient [Server-IP]\n\n");
      exit(1);
   }
   else
   {
      printf("Target: '%s'\n", argv[1]);
   }

   memset(&dest, 0, sizeof(dest));                /* zero the struct */
   dest.sin_family = AF_INET;
   dest.sin_addr.s_addr = inet_addr( argv[1] ); /* set destination IP number - localhost, 127.0.0.1*/ 
   dest.sin_port = htons(PORTNUM);                /* set destination port number */
 
   if(connect(in_socket_fd, 
      (struct sockaddr *) &dest, 
      sizeof(struct sockaddr) ) != 0)
   {
      printf("conect() failed: %s\n", strerror(errno));
      return 2;
   }
   printf("Connected to %s\n", inet_ntoa(dest.sin_addr));
  
   /******************************************************************************
   Main loop for UNIX to INET forwarding
   *****************************************************************************/
   do
   {
      nbytes = read(in_socket_fd, rcv_buf, MAXRCVLEN);
      rcv_buf[nbytes] = '\0';
      if (nbytes < 0)
      {
         printf("error on read()%s\n", strerror(errno) );
      }
      if (nbytes > 0)
      {
         printf("%s\n", rcv_buf);
      }
   } while (nbytes >= 0);

   close(in_socket_fd);
   return EXIT_SUCCESS;
}