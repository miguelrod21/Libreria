#include <stdio.h>
#include <unistd.h>
#include "parser.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main(void) {
	char buf[1024];
	tline * line;
	int i,j,n, w;
	pid_t pid;
	int p[2];
	int **pipes;
	int *pidHijos;

	printf("msh> ");	
	while (fgets(buf, 1024, stdin)) {
		line = tokenize(buf);
		if (line==NULL) {
			continue;
		}
		if(line->ncommands == 1){
			pid = fork();
			if(pid<0){
							fprintf(stderr,"Fallo fork \n%s\n",strerror(errno));
			}else if(pid==0){// Código HIJO

				if (line->redirect_input != NULL) {
					printf("redirección de entrada: %s\n", line->redirect_input);
					int entrada;
					entrada = open(line->redirect_input,O_RDONLY);
					if(entrada!=-1){
						dup2(entrada,0);
					}else{
						fprintf(stderr, "%s: Error %d.\nError al leer fichero de entrada. \n",line->redirect_input,errno);
						exit(0);
					}
				}
				if (line->redirect_output != NULL) {
					printf("redirección de salida: %s\n", line->redirect_output);
					int salida;
					salida = open(line->redirect_output,O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if(salida<0){
						fprintf(stderr, "%s: Error %d.\nError al leer fichero de salida. \n",line->redirect_output, errno);
						exit(0);
					}else{
						dup2(salida,1);
					}
				}
				if (line->redirect_error != NULL) {
					printf("redirección de error: %s\n", line->redirect_error);
				}
				if (line->background) {
					printf("comando a ejecutarse en background\n");
				} 

				if((line->commands[i].filename)!=NULL){
					execvp(line->commands[0].filename,line->commands[0].argv);
				}else{
					printf("%s: No se encuentra el mandato\n", *line->commands[0].argv);
				}
			}else{ // Código PADRE
				waitpid(pid,NULL,0);
			}
		}else if(line->ncommands == 2){ //Más de 1 comando ------------------------------------------------------------------------------

			pipe(p);
			pid =fork();
			if(pid<0){
				printf("Ha fallado el fork");
				return 1;
			}else if(pid==0){ //codigo del hijo1
				if (line->redirect_input != NULL) {
					printf("redirección de entrada: %s\n", line->redirect_input);
					int entrada;
					entrada = open(line->redirect_input,O_RDONLY);
					if(entrada!=-1){
						dup2(entrada,0);
					}else{
						fprintf(stderr, "%s: Error %d.\nError al leer fichero de entrada. \n",line->redirect_input,errno);
						exit(0);
					}
				}				
				close(p[0]);
				dup2(p[1],1);
				execv(line->commands[0].filename, line->commands[0].argv);
			}else{ //codigo padre
				pid = fork();
				if(pid == 0){ //codigo hijo2
					if (line->redirect_output != NULL) {
					printf("redirección de salida: %s\n", line->redirect_output);
					int salida;
					salida = open(line->redirect_output,O_WRONLY | O_CREAT | O_TRUNC, 0644);
					if(salida<0){
						fprintf(stderr, "%s: Error %d.\nError al leer fichero de salida. \n",line->redirect_output, errno);
						exit(0);
					}else{
						dup2(salida,1);
					}
				}
					close(p[1]);	
					dup2(p[0],0);
					execv(line->commands[1].filename, line->commands[1].argv);
				}else{ //codigo padre
					close(p[0]);
					close(p[1]);
					wait(NULL);
					wait(NULL);
				}
			}
		}else{ //Más de 2 comandos
			pidHijos = malloc(line->ncommands * sizeof(int)); 
			pipes = (int **) malloc ((line->ncommands-1) * sizeof(int *)); //Reservamos memoria para la matriz de pipes
			for(i=0;i<line->ncommands;i++){
				pipes[i] = (int *) malloc (sizeof(int)*2); 
			}
			for(i = 0; i< line->ncommands;i++){
				pid = fork();
				if(pid<0){
					fprintf(stderr, "Ha fallado el fork. %d\n", errno);
				}else if(pid==0){
					if(i==0){
						printf("Soy el primer hijo, comando: %s\n", line->commands[i].filename);
						if (line->redirect_input != NULL) {
							printf("redirección de entrada: %s\n", line->redirect_input);
							int entrada;
							entrada = open(line->redirect_input,O_RDONLY);
							if(entrada!=-1){
								dup2(entrada,0);
							}else{
								fprintf(stderr, "%s: Error %d.\nError al leer fichero de entrada. \n",line->redirect_input,errno);
								exit(0);
							}
						}				
						close(pipes[0][0]);
						dup2(pipes[0][1],1);
					}else if(i==1){
						printf("Soy el segundo hijo, comando: %s\n", line->commands[i].filename);
						close(pipes[0][1]);
						dup2(pipes[0][0],0);
						close(pipes[1][0]);
						dup2(pipes[1][1],1);
					}else{
						printf("Soy el tercer hijo, comando: %s\n", line->commands[i].filename);
						
						close(pipes[1][1]);	
						dup2(pipes[1][0],0);
					}
					execv(line->commands[i].filename, line->commands[i].argv);
				}else{
					close(pipes[0][0]);
					close(pipes[0][1]);
					close(pipes[1][0]);
					close(pipes[1][1]);
					wait(NULL);
					wait(NULL);
					wait(NULL);
				}
			}
			printf("Todos los hijos han acabado \n");
		}
		printf("msh> ");	
	}
	printf("\n");
	return 0;
}
