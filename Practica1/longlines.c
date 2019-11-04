#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
int longlines(int n1){
	int longitud=0;
	int cont=0; // Lineas leidas por stdin
	int num=0; // Encuentra la pos en la que escribir
	int i=0;
	bool found=false;
	int max=0;
	char buffer [1024];
	char ** lineas = (char **) malloc (n1*sizeof(char*));
	for(int i=0;i<=n1;i++){
		lineas[i] = (char*) malloc (sizeof(char)*1024);
	}
	while(fgets(buffer,1024,stdin)!=NULL){
		longitud = strlen(buffer);
		
	}




/*
	while(fgets(buffer,1024,stdin)!=NULL){
		longitud = strlen(buffer);
			while(i>=0 && found==false){
				if((longitud>=strlen(lineas[i]))){
				printf("Era mayor, i= %d\n",i);
					if(i!=n1-1){
						for(int j=i;j<0;j--){
							strcpy(lineas[j+1],lineas[j]);
						}
					}
					strcpy(lineas[i],buffer);
					printf("Insertado en: %d\n", i);
					for(int k=0;k<n1;k++){
						printf("%s",*(lineas+k));
					}		
					found=true;
					i--;
				}
			}
			if(!found){
				strcpy(lineas[cont],buffer);
			}*/
			printf("iteracion: %d\n", cont);
			for(int k=0;k<n1;k++){
				printf("%s",*(lineas+k));
			}
	found=false;
	cont++;		
	}
		
	for(int k=0;k<n1;k++){
		printf("%s",*(lineas+k));
	}
	free(lineas);
}
