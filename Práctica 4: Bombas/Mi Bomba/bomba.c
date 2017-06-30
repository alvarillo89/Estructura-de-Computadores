//////////////////////////////////////////////////////////////
// Compilación.
// gcc -m32 -o bomba bomba.c
/////////////////////////////////////////////////////////////

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <ctype.h>

char contrasenia[] = "krodpxqgr";
int codigo = 8001;

///////////////////////////////////////////////////////////////

int cifrar(char *respuesta){
	int tam = strlen(respuesta);
	char res[tam];

	for(int i = 0 ; i < tam; ++i)
		res[i] = respuesta[i] + 3;

	return(strncmp(contrasenia, res, strlen(contrasenia)));
}

////////////////////////////////////////////////////////////////

int descodifica(int n){
  return (n + 1008);
}

///////////////////////////////////////////////////////////////

void explosion(){
  printf("*-------------------------------*\n");
  printf("  ____   ____   ____  __  __ \n");
  printf(" |  _ \\ / __ \\ / __ \\|  \\/  |\n");
  printf(" | |_) | |  | | |  | | \\  / |\n");
  printf(" |  _ <| |  | | |  | | |\\/| |\n");
  printf(" | |_) | |__| | |__| | |  | |\n");
  printf(" |____/ \\____/ \\____/|_|  |_|\n");
  printf("*-------------------------------*\n");
  exit(-1);
}

//////////////////////////////////////////////////////////////

void desactivada(){
  printf("*-----------------*\n");
  printf(" Bomba desactivada\n");
  printf("          ,--.   \n");
  printf("       __/    *  \n");
  printf("     ,d08b.      \n");
  printf("     0088MM      \n");
  printf("     `9MMP'      \n");
  printf("*-----------------*\n");
  exit(0);
}

//////////////////////////////////////////////////////////////

int main(){
  #define TAMA 30
  #define LIMTIEMPO 10
  char respuesta[TAMA];
  int respuesta_num;
  struct timeval t_antes, t_despues;

  gettimeofday(&t_antes,NULL);

  printf("Introduce la contraseña: ");
  fgets(respuesta,TAMA,stdin);

  if(cifrar(respuesta))
    explosion();

	printf("Introduce el código numérico: ");
	scanf("%i",&respuesta_num);
	if(descodifica(respuesta_num) != codigo)
    explosion();

	gettimeofday(&t_despues,NULL);
	if(t_despues.tv_sec - t_antes.tv_sec > LIMTIEMPO)
    explosion();

  desactivada();
}
