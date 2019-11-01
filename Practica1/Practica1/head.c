#include <stdlib.h>
#include <stdio.h>
#include <string.h>
int head(int n1){
	int cont=0;
	char buffer [1024];
	char ** lineas = (char **) malloc (n1*sizeof(char*));
	for(int i=0;i<=n1;i++){
		lineas[i] = (char*) malloc (sizeof(char)*1024);
	}

	while(fgets(buffer,1024,stdin)!=NULL){
		if(cont<=n1){
			strcpy(lineas[cont],buffer);
			cont++;
		}
	}
	for(int k=0;k<n1;k++){
		printf("\n %s",*(lineas+k));
	}
	free(lineas);
}
