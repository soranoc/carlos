#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>


int creer_serveur(int port){
  //Cr�ation de la socket
  int socket_serveur ;
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
