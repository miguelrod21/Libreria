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
		printf("%s",*(lineas+k));
	}
	free(lineas);
}
int tail(int n1){
	int cont=0;
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

int longlines(int n1){
	int numero;
	int num;
	int x;
	int cont=0;
	char buffer [1024];
	char ** lineas = (char **) malloc (n1*sizeof(char*));
	for(int i=0;i<=n1;i++){
		lineas[i] = (char*) malloc (sizeof(char)*1024);
	}

	while(fgets(buffer,1024,stdin)!=NULL){
        x= strlen(buffer);
        if (cont == 0){
            strcpy(lineas[cont],buffer);
            cont++;
        }else if(cont < n1){
            	num = 0;
                while((strlen(lineas[num]) > strlen(buffer)) && (num <= cont)){
                    num++;
                }
                if (num == cont){
                    strcpy(lineas[num],buffer);
                    cont++; 
                }else{
			for(int a = (n1 - 1); a >= num; a--){
                        	strcpy(lineas[a + 1], lineas[a]);
                    	}
			strcpy(lineas[num + 1], lineas[num]);
                    strcpy(lineas[num], buffer);
                    cont++;
			
                }
        }else {
            numero = 0;
            while((strlen(lineas[numero]) >= strlen(buffer)) && (numero < n1)){
                numero++;
            }
            if (numero == n1){
                strcpy(lineas[numero],buffer);
                cont++; 
            }else{
                for(int b = (n1 - 2); b >= numero; b--){
                    strcpy(lineas[b + 1], lineas[b]);
                }
                strcpy(lineas[numero], buffer);
                cont++;
            }
        }
	}
	for(int k=0;k<n1;k++){
		printf("%s",*(lineas+k));
	}
	free(lineas);
}
