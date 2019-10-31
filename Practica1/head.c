#include <stdlib.h>
#include <stdio.h>
int head(int n1){
	char ** lineas = (char **) malloc (n1*sizeof(char*));
	for(int i=0;i<n1;i++){
		lineas[i] = (char*) malloc (sizeof(char)*1024);
	}

	for(int j=0;j<n1;j++){
		*lineas[j] = 'a';
	}
	
	for(int k=0;k<n1;k++){
		printf("\n %s",*(lineas+k));
	}
	free(lineas);
}
