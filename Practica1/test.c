#include "libreria.h"
int main(int argc, char *argv[]){
	int op;
	char *comando = argv[1];
	int n1 = atoi(argv[2]);
	if(argc!=3){
		fprintf(stderr,"Error en parámetros. Por favor introduce la opcion head, tail o longlines seguido de un número entero.\n");
		return 1;
	}else{
		if(!isdigit(*argv[2])){
			printf("El segundo parámetro no es válido. Por favor introduce un entero para el número de lineas a mostrar.\n");	
		return 1;
		}	
	}
	
	if(strcmp(comando,"head")==0){
		head(n1);
	}else if(strcmp(comando,"tail")==0){
		tail(n1);
	}else if(strcmp(comando,"longlines")==0){
		longlines(n1);
	}else{
		fprintf(stderr,"Error en el primer parámetro. Por favor introduce el comando deseado(head, tail o longlines)\n");	
		return 1;
	}
	return 0;	
}
