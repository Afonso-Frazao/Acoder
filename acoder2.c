#include <stdlib.h>
#include <stdio.h>

int main(int argc, char **argv){
	char mode;
	int i, j;
	char *finname, *foutname, *key;
	char eoutname[12]="_acoded.txt";
	char doutname[13]="_decoded.txt";
	char buff;
	FILE *fp, *fp2;

	if(argc!=4){
		fprintf(stdout, "Please write in the format: './acoder M NAME KEY'\nM - operating mode:\t'e' or 'E' for encoding\n\t\t\t'd' or 'D' for decoding\nNAME - name of the file to be encoded\nKEY - encoding key: between -32768 and 32767\n\n");
		return(1);
	}

	mode=argv[1][0];
	finname=(char*)malloc(100);
	for(i=0; argv[2][i]!=0; i++){
		finname[i]=argv[2][i];
	}
	finname[i]=0;

	key=(char*)malloc(2);
	key[0]=argv[3][0];
	key[1]=argv[3][1];

	if(mode=='e' || mode=='E'){

		foutname=(char*)malloc(108);

		for(i=0; finname[i]!='.'; i++){
			foutname[i]=finname[i];
		}
		for(j=0; j<12; j++, i++){
			foutname[i]=eoutname[j];
		}


		fp=fopen(finname, "r");
		fp2=fopen(foutname, "w+");

		while(fscanf(fp, "%c", &buff)!=EOF){
			buff^=key[0];
			buff+=key[1];
			fprintf(fp2, "%c", buff);
		}

	}
	else if(mode=='d' || mode=='D'){
		foutname=(char*)malloc(109);

		for(i=0; finname[i]!='.'; i++){
			foutname[i]=finname[i];
		}
		for(j=0; j<13; j++, i++){
			foutname[i]=doutname[j];
		}

		fp=fopen(finname, "r");
		fp2=fopen(foutname, "w+");

		while(fscanf(fp, "%c", &buff)!=EOF){
			buff-=key[1];
			buff^=key[0];
			fprintf(fp2, "%c", buff);
		}
	}
	else{
		return(1);
	}

	fclose(fp);
	fclose(fp2);

	free(finname);
	free(foutname);
	free(key);

	return 0;
}
