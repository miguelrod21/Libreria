#include "libreria.h"
int main(int argc, char *argv[]){
	if(argc!=3){
		printf("Error en parámetros. Por favor introduce la opcion head, tail o longlines seguido de un número entero.\n");
		return 1;
	}
	printf("El número pasado es: %s\n",argv[2]);
	int sol = 0;
	int n1 = atoi(argv[2]);
	int op = atoi(argv[1]);

	switch(op){
	case 1:
		sol = head(n1);
		break;
	case 2:
		sol=tail(n1);
		break;
	case 3:
		sol=longlines(n1);
		break;
	default:
		printf("Error en segundo parámetro. Por favor introduce el comando deseado.\n");
		return 1;
	}
	printf("La solucion es: %d\n",sol);
	return 0;	
}
