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
	int status;
	printf("==> ");	
	while (fgets(buf, 1024, stdin)) {
		line = tokenize(buf);
		if (line==NULL) {
			continue;
		}
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
		
		
		printf("==> ");	
	}
	return 0;
}
