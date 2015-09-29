#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>
#include <signal.h>

int creer_serveur(int port){
  //Cr�ation de la socket
  int socket_serveur ;
  int optval = 1;
  socket_serveur = socket ( AF_INET , SOCK_STREAM , 0);
  if ( socket_serveur == -1)
    {
      perror ( " socket_serveur " );
      /* traitement de l ' erreur */
    }
  
  struct sockaddr_in saddr ;
  saddr.sin_family = AF_INET ; /* Socket ipv4 */
  saddr.sin_port = htons (port); /* Port d ' �coute */
  saddr.sin_addr.s_addr = INADDR_ANY ; /* �coute sur toutes les interfaces */

  //Application de setsockopt sur la socket serveur
  if (setsockopt(socket_serveur, SOL_SOCKET , SO_REUSEADDR , &optval , sizeof(int)) == -1)
    {
      perror("Can not set SO_REUSEADDR option");
    }

  //Bind de la socket
  if ( bind ( socket_serveur , ( struct sockaddr *)& saddr , sizeof ( saddr )) == -1)
    {
      perror ( " bind socket_serveur " );
      /* traitement de l ' erreur */
    }
  
  if ( listen ( socket_serveur , 10) == -1)
    {
      perror ( " listen socket_serveur " );
      /* traitement d ' erreur */
    }
  return socket_serveur;
}

void initialiser_signaux(){
  if (signal(SIGPIPE , SIG_IGN) ==  SIG_ERR)
    {
      perror("signal");
    }
}

