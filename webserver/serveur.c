#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>

enum  http_method {
  HTTP_GET ,
  HTTP_UNSUPPORTED ,
};

typedef  struct
{
  enum  http_method  method;
  int   major_version;
  int   minor_version;
  char *url;
} http_request;

char *fgets_or_exit(char *buffer , int size , FILE *stream)
{
  char* str = fgets(buffer, size, stream);
  if(str==NULL)
    {
      printf("La connexion est interrompue !");
      exit(1);
    }
  return str;
}

int parse_http_request(const char *request_line , http_request *request)
{
  char* token ="";
  char* reqdup = strdup(request_line);
  int ret = 1;
  int tkCounter = 0;
  token = strtok(reqdup, " ");
  request->method = HTTP_UNSUPPORTED;

  while(token)
    {
      ++tkCounter;
      if(tkCounter==1 && strcmp(token,"GET")==0)
	{
	  
	  request->method = HTTP_GET;
	}
      if(tkCounter==2)
	{
	  request->url = token;
	}	      
      if(tkCounter > 3)
	{
	  ret = 0;
	}
      if(tkCounter == 3)
	{
	  if(strcmp(token,"HTTP/1.0\r\n")==0)
	    {
	      request->major_version = 1;
	      request->minor_version = 0;
	    }
	  else if (strcmp(token,"HTTP/1.1\r\n")==0)
	    {
	      request->major_version = 1;
	      request->minor_version = 1;
	    }
	  else
	    {
	      ret = 0;
	    }
	}
      token=strtok(NULL," ");
    } 
  if(tkCounter < 3 )
    {
      ret = 0;
    }
  return ret;
}

void skip_headers(FILE *client)
{
  int ligneVide = 0;
  char buffer[80];
  char* str="";
  while(ligneVide==0)
    {
      str = fgets_or_exit(buffer, 80, client);
      if(str!= NULL && (strcmp(str, "\r\n")==0 || strcmp(str, "\n")==0))
	{
	  ligneVide=1;
	}
    }
}

void send_status(FILE *client , int code , const char *reason_phrase)
{
  fprintf(client, "HTTP/1.1 %d %s\n", code, reason_phrase);
}

void send_response(FILE *client , int code , const  char *reason_phrase , const  char *message_body)
{
  send_status(client, code, reason_phrase);
  fprintf(client, "Content-Length : %d\r\n\r\n", (int)strlen(message_body));
  fprintf(client, message_body);
  fflush(client);  
}

