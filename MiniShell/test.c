#include <stdio.h>
#include <unistd.h>
#include "parser.h"
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
int main(void) {
	char buf[1024];
	tline * line;
	int i,j,n,w,status;
	pid_t pid;
	int p[2];
	int **pipes;
	int entrada, salida, errpipe;

	printf("msh> ");	
	while (fgets(buf, 1024, stdin)) {
		line = tokenize(buf);
		if (line==NULL) {
			continue;
		}
		if(line->ncommands == 1){
			
            if (strcmp(line -> commands[0].argv[0], "cd") == 0){
                if (line -> commands[0].argc == 1){
                    chdir(getenv("HOME"));                   
                }else{
                    if (chdir(line -> commands[0].argv[1]) != 0){
                        printf("Ese directorio no existe. \n");
                    }
                }
            }
            else{
                pid = fork();
			    if(pid<0){
							    fprintf(stderr,"Fallo fork \n%s\n",strerror(errno));
			    }else if(pid==0){// Código HIJO

				    if (line->redirect_input != NULL) {
					    printf("redirección de entrada: %s\n", line->redirect_input);
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
					    salida = open(line->redirect_output,O_WRONLY | O_CREAT | O_TRUNC, 0644);
					    if(salida<0){
						    fprintf(stderr, "%s: Error %d.\nError al leer fichero de salida. \n",line->redirect_output, errno);
						    exit(0);
					    }else{
						    dup2(salida,1);
					    }
				    }

				    if((line->commands[i].filename)!=NULL){
					    execvp(line->commands[0].filename,line->commands[0].argv);
				    }else{
					    printf("%s: No se encuentra el mandato\n", *line->commands[0].argv);
				    }
			    }else{ // Código PADRE
				    wait(NULL);
			    }
            }

		}else if(line->ncommands == 2){ //2 comandos entrada + salida------------------------------------------------------------------------------

			pipe(p);
			pid =fork();
			if(pid<0){
				printf("Ha fallado el fork");
				return 1;
			}else if(pid==0){ //codigo del hijo1
				if (line->redirect_input != NULL) {
					printf("redirección de entrada: %s\n", line->redirect_input);
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
					if((line->commands[i].filename)!=NULL){
						execvp(line->commands[1].filename,line->commands[1].argv);
					}else{
						printf("%s: No se encuentra el mandato\n", *line->commands[i].argv);
					}
				}else{ //codigo padre
					close(p[0]);
					close(p[1]);
					wait(NULL);
					wait(NULL);
				}
			}
		}else{ //Más de 2 comandos
			pipes = (int **) malloc ((line->ncommands-1) * sizeof(int *)); 
			for(i=0;i<line->ncommands-1;i++){
				pipes[i] = (int *) malloc (sizeof(int)*2); 
				errpipe = pipe(pipes[i]);
				if(errpipe<0){
					fprintf(stderr, "Error en el pipe: %d. Error %d\n",i, errno );
				}
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
							entrada = open(line->redirect_input,O_RDONLY);
							if(entrada!=-1){
								dup2(entrada,0);
							}else{
								fprintf(stderr, "%s: Error %d.\nError al leer fichero de entrada. \n",line->redirect_input,errno);
								exit(0);
							}
						}		
						for(j=i+1; j<line->ncommands-1; j++){ 
							close(pipes[j][1]);
							close(pipes[j][0]);
						}
						close (pipes[0][0]);
                        close (pipes[1][0]);
                        close (pipes[1][1]);
						dup2(pipes[0][1],1);
						
					}else if(i>0 && i<line->ncommands-1){
						printf("Soy un hijo del medio[%d], comando: %s\n", i,line->commands[i].filename);
						if(i==1){
							for(j=0; j<i-1; j++){ 
								close(pipes[j][1]);
								close(pipes[j][0]);	
							}
						}
						if(i!=1){ 
							for(j=0; j<i-1; j++){ 
								close(pipes[j][1]);
								close(pipes[j][0]);
							}
							
						}
						close(pipes[i-1][1]);
						dup2(pipes[i-1][0],0);
						close(pipes[i][0]);
						dup2(pipes[i][1],1);
					}else{
						printf("Soy el ultimo hijo, comando: %s\n", line->commands[i].filename);
						if (line->redirect_output != NULL) {
							printf("redirección de salida: %s\n", line->redirect_output);
							salida = open(line->redirect_output,O_WRONLY | O_CREAT | O_TRUNC, 0644);
							if(salida<0){
								fprintf(stderr, "%s: Error %d.\nError al leer fichero de salida. \n",line->redirect_output, errno);
								exit(0);
							}else{
								dup2(salida,1);
							}
						}
						for(j=0; j<line->ncommands-2; j++){ 
							close(pipes[j][1]);
							close(pipes[j][0]);
						}
						close(pipes[i-1][1]);
						dup2(pipes[i-1][0],0);
					}
					if((line->commands[i].filename)!=NULL){
						execvp(line->commands[i].filename,line->commands[i].argv);
					}else{
						printf("%s: No se encuentra el mandato\n", *line->commands[i].argv);
					}
				}else{ //el padre
					
				}
				
			}
			for(j=0; j<line->ncommands-1; j++){ 
				close(pipes[j][1]);
				close(pipes[j][0]);
			}
			printf("Esperando... \n");
			for(i=0;i<line->ncommands;i++){
				wait(NULL);
			}
			for(i=0;i<line->ncommands-1;i++){
				free(pipes[i]);
			}
			free(pipes);
		}
		
		printf("msh> ");	
	}
	printf("\n");
	return 0;
}
