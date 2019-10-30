#include "libreria.h"
int main(int argc, const char *argv[]){
	int n1 = atoi(argv[1]);
	int n2 = atoi(argv[2]);

	int sol=0;
sol=head(n1,n2);
printf("%d",sol);
sol=tail(n1,n2);
printf("%d",sol);
sol=longlines(n1,n2);
printf("%d",sol);

return 0;
}
