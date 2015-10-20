#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>

#include "socket.h"
#include "serveur.h"

int main()
{
  int socket_serveur;
  int socket_client;
  char buffer[80];
  int pid;
  FILE* file;
  char *str;
  int bad_request = 0;
  http_request request;

  initialiser_signaux();
  socket_serveur=creer_serveur(8080);
  
  while(1)
    {
      socket_client = accept ( socket_serveur , NULL , NULL );
      if ( socket_client == -1)
	{
	  perror (" accept ");
	  /* traitement d ' erreur */
	}
      
      file = fdopen(socket_client, "w+");
      if(file==NULL)
	{
	  perror ("fdopen error");
	}
      
      pid = fork();
      if(pid==-1)
	{
	  perror(" Erreur Fork serveur ");
	}
      if(pid==0)
	{	  
	  str = fgets_or_exit(buffer, 80, file);
	  fflush(file);
	  bad_request = parse_http_request(str, &request);
	  skip_headers(file);
	  if (bad_request == 0)
	    send_response(file , 400, "Bad Request", "Bad request\r\n");
	  else if (request.method  ==  HTTP_UNSUPPORTED)
	    send_response(file , 405, "Method Not Allowed", "Method Not Allowed\r\n");
	  else if (strcmp(request.url, "/") == 0)
	    send_response(file , 200, "OK", "Bienvenue chez <carlos> !\r\n");
	  else
	    send_response(file , 404, "Not Found", "Not Found\r\n");
	  exit(0);		  
	}
      else
	{
	  close(socket_client);
	}
    }
  return 0; 
}


