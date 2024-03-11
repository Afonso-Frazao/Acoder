#include <stdlib.h>
#include <stdio.h>

char encrypt(char num, char *key){

}

int main(int argc, char **argv){
	char mode;
	int i, j;
	long keyn;
	char key1, key2, key3, key4, key5, key6;
	char *finname, *foutname, *key;
	char eoutname[12]="_acoded.txt";
	char doutname[13]="_decoded.txt";
	char buff;
	FILE *fp, *fp2;

	if(argc!=4){
		fprintf(stdout, "Please write in the format: './acoder M NAME KEY'\nM - operating mode:\t'e' or 'E' for encoding\n\t\t\t'd' or 'D' for decoding\nNAME - name of the file to be encoded\nKEY - encoding key: between -2147483648 and 2147483647\n\n");
		return(1);
	}

	mode=argv[1][0];
	finname=(char*)malloc(100);
	for(i=0; argv[2][i]!=0; i++){
		finname[i]=argv[2][i];
	}
	finname[i]=0;


	keyn=atoi(argv[3]);
	key=&keyn;

	printf("keyn: %ld\n", keyn);

	printf("key: %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7]);

	if(mode=='e' || mode=='E'){ //encrypt

		foutname=(char*)malloc(108);

		for(i=0; finname[i]!='.'; i++){
			foutname[i]=finname[i];
		}
		for(j=0; j<12; j++, i++){
			foutname[i]=eoutname[j];
		}


		fp=fopen(finname, "r");
		fp2=fopen(foutname, "w+");

		key1=key[0];
		key2=key[1];
		key3=key[2];
		key4=key[3];
		key5=key[4];
		key6=key[5];
		while(fscanf(fp, "%c", &buff)!=EOF){
			buff^=key1;
			buff+=key2;
			buff^=key3;
			buff=~buff;
			buff-=key4;
			buff^=key5;
			buff+=key6;
			buff=~buff;

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
			buff+=key[3];
			buff=~buff;
			buff^=key[2];
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

	return 0;
}
