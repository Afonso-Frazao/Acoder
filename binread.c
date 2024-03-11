#include <stdio.h>

int main(int argc, char *argv[]){
	char buff;
	FILE *fp;

	fp=fopen(argv[1], "r");

	while(fscanf(fp, "%c", &buff)!=EOF){
		printf("%hhd ", buff);
	}

	fclose(fp);

	return 0;
}
