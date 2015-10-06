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
  //const char * message_bienvenue = " Bonjour , bienvenue sur mon serveur \n Cum saepe multa, tum memini domi in hemicyclio sedentem, ut solebat, cum et ego essem una et pauci admodum familiares, in eum sermonem illum incidere qui tum forte multis erat in ore. Meministi enim profecto, Attice, et eo magis, quod P. Sulpicio utebare multum, cum is tribunus plebis capitali odio a Q. Pompeio, qui tum erat consul, dissideret, quocum coniunctissime et amantissime vixerat, quanta esset hominum vel admiratio vel querella. Quid enim tam absurdum quam delectari multis inanimis rebus, ut honore, ut gloria, ut aedificio, ut vestitu cultuque corporis, animante virtute praedito, eo qui vel amare vel, ut ita dicam, redamare possit, non admodum delectari? Nihil est enim remuneratione benevolentiae, nihil vicissitudine studiorum officiorumque iucundius.Ipsam vero urbem Byzantiorum fuisse refertissimam atque ornatissimam signis quis ignorat? Quae illi, exhausti sumptibus bellisque maximis, cum omnis Mithridaticos impetus totumque Pontum armatum affervescentem in Asiam atque erumpentem, ore repulsum et cervicibus interclusum suis sustinerent, tum, inquam, Byzantii et postea signa illa et reliqua urbis ornanemta sanctissime custodita tenuerunt. \n" ;
  int socket_serveur;
  int socket_client;
  char buffer[80];
  int pid;
  FILE* file;
  char *str;
  char* token;
  int tkCounter;
  int ok = 1;

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
	  sleep(1);
	  //write(socket_client, message_bienvenue , strlen(message_bienvenue)+1);
	  str = fgets(buffer, 80, file);
	  fflush(file);
	  token = strtok(str, " ");
	  while(token)
	    {
	      ++tkCounter;
	      if(tkCounter==1 && strcmp(token,"GET")!=0)
		{
		  ok = 0;
		}
	      if(tkCounter > 3)
		{
		  ok = 0;
		}
	      if(tkCounter == 3 && (strcmp(token,"HTTP/1.0\r\n")!=0 || strcmp(token,"HTTP/1.1\r\n")!=0))
		{
		  ok = 0;
		}
	      token=strtok(NULL," ");
	    }
	  if(tkCounter < 3 )
	    {
	      ok = 0;
	    }
	  
	  if(ok==1)
	    {
	      while(str != NULL)
		{
		  fprintf(stdout, "<carlos> %s", buffer);
		  fflush(file);
		  str = fgets(buffer, 80, file);
		  fflush(file);
		}
	    }
	}
      
      else
	{
	  close(socket_client);
	}
      return 0;
    }
}
