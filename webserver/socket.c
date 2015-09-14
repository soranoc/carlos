#include <sys/types.h>         
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <string.h>



int creer_serveur(){
  
  //Création de la socket
  int socket_serveur ;
  socket_serveur = socket ( AF_INET , SOCK_STREAM , 0);
  if ( socket_serveur == -1)
    {
      perror ( " socket_serveur " );
      /* traitement de l ' erreur */
    }
  
  struct sockaddr_in saddr ;
  saddr.sin_family = AF_INET ; /* Socket ipv4 */
  saddr.sin_port = htons (8080); /* Port d ' écoute */
  saddr.sin_addr.s_addr = INADDR_ANY ; /* écoute sur toutes les interfaces */
  if ( bind ( socket_serveur , ( struct sockaddr *)& saddr , sizeof ( saddr )) == -1)
    {
      perror ( " bind socker_serveur " );
      /* traitement de l ' erreur */
    }
  
  if ( listen ( socket_serveur , 10) == -1)
    {
      perror ( " listen socket_serveur " );
      /* traitement d ' erreur */
    }
  return 0;

}
/* Utilisation de la socket serveur */
/* On peut maintenant dialoguer avec le client */
int boucle_serveur(int socket_serveur){
  int socket_client ;
  socket_client = accept ( socket_serveur , NULL , NULL );
  if ( socket_client == -1)
    {
      perror (" accept ");
      /* traitement d ' erreur */
    }
  
  const char * message_bienvenue = " Bonjour , bienvenue sur mon serveur \n " ;
  write(socket_client, message_bienvenue , strlen(message_bienvenue));
 return 0;
}
