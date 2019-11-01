#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int tail(int n1){
	int cont=0;
	int numLineas=0;
	char buffer [1024];
	char ** lineas = (char **) malloc (n1*sizeof(char*));
	for(int i=0;i<=n1;i++){
		lineas[i] = (char*) malloc (sizeof(char)*1024);
	}

	while(fgets(buffer,1024,stdin)!=NULL){
		if(cont<n1){
			strcpy(lineas[cont],buffer);
			cont++;
		}else{
			for(int i=0;i<n1;i++){
				strcpy(lineas[i],lineas[i+1]);
			}
		strcpy(lineas[cont],buffer);
		}
		
	}
	for(int k=0;k<n1+1;k++){
		printf("%s",*(lineas+k));
	}
	free(lineas);
}
