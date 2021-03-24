#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

void dibujar_ahorcado(int i,char *b,char *c);
int ahorcado(char *palabra);

void error(const char *msg){
    perror(msg);
    exit(0);
}

int main(){
    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[256];
    char NombreJ[50];
    FILE * PuntajesJug;

    PuntajesJug=fopen("PunatajesDeJugadores.txt","a");

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) 
        error("ERROR opening socket");
    
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }
    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr, 
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(3500);    //usamos el puerto 3500
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) 
        error("ERROR connecting");

    system("clear");

    printf("\n\tA H O R C A D O\n\nPor favor seleccione una dificultad:");
    printf("\na - facil\nb - intermedio\nc - dificil\n");
    bzero(buffer,256);
    fgets(buffer,255,stdin);
    
    while(*buffer!='a' && *buffer!='b' && *buffer!='c'){
        printf("Por favor selecciona una opcion valida\na - facil\nb - intermedio\nc - dificil\n");
        bzero(buffer,256);
        fgets(buffer,255,stdin);
    }
    
    n = write(sockfd,buffer,strlen(buffer));
    if (n < 0) 
         error("ERROR writing to socket");
    bzero(buffer,256);
    n = read(sockfd,buffer,255);    //obtenemos la palabra por adivinar
    if (n < 0) 
         error("ERROR reading from socket");
    system("clear");

    int marcador=10-ahorcado(buffer);
    /*marcador=10-intentos. Yo digo que multipliquemos marcador x tiempo o algo 
    así pa determinar el puntaje*/
    fflush(stdin);
    printf("\n\tIntroduce tu nombre de jugador:\n>");//su nombresin
    scanf("%[^\n]", NombreJ);
    
    fputs("Jugador: ",PuntajesJug);
    fputs(NombreJ,PuntajesJug);
    
    printf("Jugador: %s\nPuntos: %d\n",NombreJ, marcador);

    fputs("    Puntaje:",PuntajesJug);
    
    fputs("\0",PuntajesJug);
    
    close(sockfd);
    fclose(PuntajesJug);
    return 0;
}

void dibujar_ahorcado(int i,char *b,char *c){
    switch(i){
        case 1:
        printf("\n\t\t\t|\n\t\t\t|\n\n\n");
        break;
        case 2:
        printf("\n\t\t\t|\n\t\t\t|\n\t\t\t|\n\n");
        break;
        case 3:
        printf("\n\t\t\t|\n\t\t\t|\n\t\t\t| \n\t\t\t|\n");
        break;
        case 4:
        printf("\t\t\t_\n\t\t\t|\n\t\t\t|\n\t\t\t| \n\t\t\t|");
        break;
        case 5:
        printf("\t\t\t__\n\t\t\t|\n\t\t\t|\n\t\t\t| \n\t\t\t|");
        break;
        case 6:
        printf("\t\t\t___\n\t\t\t|\n\t\t\t|\n\t\t\t| \n\t\t\t|");
        break;
        case 7:
        printf("\t\t\t___\n\t\t\t|  |\n\t\t\t|\n\t\t\t| \n\t\t\t|");
        break;
        case 8:
        printf("\t\t\t___\n\t\t\t|  |\n\t\t\t|  0\n\t\t\t| \n\t\t\t|");
        break;
        case 9:
        printf("\t\t\t___\n\t\t\t|  |\n\t\t\t|  0\n\t\t\t| /|\\ \n\t\t\t|");
        break;
        case 10:
        printf("\t\t\t___\n\t\t\t|  |\n\t\t\t|  0\n\t\t\t| /|\\ \n\t\t\t| / \\ ");
        break;
        default:
        break;
    }
    printf("\n\n\n\n\t\t\t%s\t\t\t Numero de intentos -> %d\n\n\n\n\t\tLETRAS INTRODUCIDAS >>> %s ",b,i,c);
}

int ahorcado(char *palabra){
    char introducidos[11]=" ";
    char *str=NULL;
    char letra_intro;
    int hubo=0,x,intentos=0;

    str = (char *)malloc(sizeof(char) * (strlen(palabra)+1));

    for(x=0;x<strlen(palabra);x++)
        *(str + x)='_';
    str[x]='\0';

    dibujar_ahorcado(intentos,str,introducidos);

    //BUCLE PRINCIPAL
    while (1){
        if(intentos>=10){
            //dibujar_ahorcado(intentos,str,introducidos);
            printf("\n\n\n\t\tLo siento has fallado :( \n\n\t\tLa palabra era: '%s' \n\n\n\n\n",palabra);
            //intentos=-intentos;
            break;
        }
        hubo=0;
        //LECTURA DE LETRA
        printf("\n\n\n\n\t\tIntroduce una letra >>> ");
        scanf(" %c",&letra_intro);
        system("clear");
        //RECORRO LA PALABRA CORRECTA PARA COMPARARLA CON LA INTRODUCIDA POR EL USUARIO
        for(x=0;x<strlen(palabra);x++){
            if(palabra[x]==letra_intro){
                //SI ES LA MISMA LETRA PONGO EN EL ARRAY DE BARRABAJA LA LETRA Y INDICO QUE HUBO COINCIDENCIAS
                *(str + x)  = letra_intro;
                hubo=1;
            }
        }
        //printf("%s,%s\n",str,palabra);        //para mostrar la palabra por adivinar
        if(strcmp(palabra,str)==0){
            printf("\n\n\n\t\tFelicidades, has acertado!!! La palabra es: '%s'\n\n\n\n\n",palabra);
            break;
        }
        
        if(hubo==0){
            printf("Lo siento, intenta con otra letra\n");
            intentos++;
            introducidos[intentos]=letra_intro;
        }
        dibujar_ahorcado(intentos,str,introducidos);
    }
    free(str);
    return intentos;
}