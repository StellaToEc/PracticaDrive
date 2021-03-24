#include <stdio.h>

void Puntajes();

int main()
{
	Puntajes();
}

void Puntajes()
{
	char NombreJug[3];
	FILE * PuntajesJug;

	printf("\n\nInserta tu nombre de jugador (3 letras):\n>");
	scanf("%[^\n]",&NombreJug);

	PuntajesJug = fopen("registroDePuntajes.txt", "wt");
	fputs(NombreJug,PuntajesJug);

	fclose(PuntajesJug);
	printf("su nombre es:%s",NombreJug);
}