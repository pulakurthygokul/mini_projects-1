#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

struct addrinfo hints, *infoptr; // So no need to use memset global variables

int main() 
{
  hints.ai_family = AF_INET; // AF_INET means IPv4 only addresses

  int result = getaddrinfo("www.bbc.com", NULL, &hints, &infoptr);
  if (result) 
  {
      fprintf(stderr, "getaddrinfo: %s\nerror: %d\n", gai_strerror(result), result);
      exit(1);
  }

  struct addrinfo *p;
  char host[256],service[256];

  for(p = infoptr; p != NULL; p = p->ai_next) 
  {

      getnameinfo(p->ai_addr,p->ai_addrlen, host, sizeof(host), service,sizeof(service),NI_NUMERICHOST );
      puts(host);
  }
    
  freeaddrinfo(infoptr);
  return 0;
}
