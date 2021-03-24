#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <time.h>
#include <string.h>

void error(const char *msg){
   perror(msg);
   exit(1);
}

int main(int argc, char *argv[]){
   char easy[][10] = {"corto","mesa","reloj","papa","cereal","vela","pluma","pala","tela","tasa"};
   char medium[][12] = {"amarrar","pantalla","escribir","carretera","construir","jirafa",
                  "encontrar","ventana","perfume","llamada"};
   char hard[][25] = {"anticonstitucionalmente","electrocardiograma","fotosintesis","electrodomestico",
                  "caleidoscopio","fisioterapia","discapacidad","procedimiento","pasteurizar","transgresor"};
   
   int sockfd, newsockfd;
   socklen_t clilen;
   char buffer[256];
   struct sockaddr_in direcc, cli_addr;
   int n;

   sockfd = socket(AF_INET, SOCK_STREAM, 0);
   if (sockfd < 0) 
      error("ERROR opening socket");

   bzero((char *) &direcc, sizeof(direcc));   //no entiendo esta
   direcc.sin_family = AF_INET;
   direcc.sin_addr.s_addr = INADDR_ANY;
   direcc.sin_port = htons(3500);    //usamos el puerto 3500

   if (bind(sockfd, (struct sockaddr *) &direcc, sizeof(direcc)) < 0) 
      error("ERROR on binding");

   if(listen(sockfd,4)<0)
      error("ERROR on listen");

   clilen = sizeof(cli_addr);
   while(1){
      newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);
      if (newsockfd < 0) 
         error("ERROR on accept");
      bzero(buffer,256);
      n = read(newsockfd,buffer,255);
      if (n < 0) error("ERROR reading from socket");

      printf("Here is the message: %s\n",buffer);

      srand(time(NULL));
      char index = rand()%10;
    
      if(*buffer=='a')
         n = write(newsockfd,easy[index],strlen(easy[index]));
      else if(*buffer=='b')
         n = write(newsockfd,medium[index],strlen(medium[index]));
      else if(*buffer=='c')
         n = write(newsockfd,hard[index],strlen(hard[index]));
      else
         n = write(newsockfd,"Error en la seleccion",21);
      
      if (n < 0) error("ERROR writing to socket");
   }
   close(newsockfd);
   close(sockfd);
   return 0; 
}