#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <signal.h>

#include "socket.h"

int main()
{
  const char * message_bienvenue = " Bonjour , bienvenue sur mon serveur \n Cum saepe multa, tum memini domi in hemicyclio sedentem, ut solebat, cum et ego essem una et pauci admodum familiares, in eum sermonem illum incidere qui tum forte multis erat in ore. Meministi enim profecto, Attice, et eo magis, quod P. Sulpicio utebare multum, cum is tribunus plebis capitali odio a Q. Pompeio, qui tum erat consul, dissideret, quocum coniunctissime et amantissime vixerat, quanta esset hominum vel admiratio vel querella. Quid enim tam absurdum quam delectari multis inanimis rebus, ut honore, ut gloria, ut aedificio, ut vestitu cultuque corporis, animante virtute praedito, eo qui vel amare vel, ut ita dicam, redamare possit, non admodum delectari? Nihil est enim remuneratione benevolentiae, nihil vicissitudine studiorum officiorumque iucundius.Ipsam vero urbem Byzantiorum fuisse refertissimam atque ornatissimam signis quis ignorat? Quae illi, exhausti sumptibus bellisque maximis, cum omnis Mithridaticos impetus totumque Pontum armatum affervescentem in Asiam atque erumpentem, ore repulsum et cervicibus interclusum suis sustinerent, tum, inquam, Byzantii et postea signa illa et reliqua urbis ornanemta sanctissime custodita tenuerunt. \n" ;
  int socket_serveur;
  int socket_client;
  int taille;
  char buffer[10];

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

      sleep(1);
      write(socket_client, message_bienvenue , strlen(message_bienvenue)+1);
      
      taille=read(socket_client, buffer, 10);
      while(taille!=0 && taille!=-1)
	{
	  write(socket_client, buffer, taille);
	  printf("%d\n", taille);
	  taille=read(socket_client, buffer, 10);
	}
    }
  
  return 0;
}
