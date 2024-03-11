#include <stdlib.h>
#include <stdio.h>

char module(char num){
	if(num<0){
		return -num;
	}
	else{
		return num;
	}
}

char appop(unsigned char num, unsigned char key, char op){ //apply operation
	switch(op){
		case 0:
			num+=key;
		case 1:
			num-=key;
		case 2:
			num^=key;
		case 3:
			printf("num before shift '%hhd': %hhd\n", (key&7), num);
			printf("left: %hhd\nright: %hhd\n", num<<(key&7), (num>>((sizeof(char)*8)-(key&7))));
			if(num>=0){
				num=(num<<(key&7)) | (num>>((sizeof(char)*8)-(key&7)));
			}
			printf("num after shift '%hhd': %hhd\n", (key&7), num);
			if((key&15)==15){
				num=~num;
			}
	}

	return num;
}

int main(int argc, char **argv){
	char mode;
	int i, j;
	long keyn;
	char *finname, *foutname, *key;
	char keykey, opord;
	short opslc;
	char *ops;
	char eoutname[12]="_acoded.txt";
	char doutname[13]="_decoded.txt";
	char buff;
	FILE *fp, *fp2;

	if(argc!=4){
		fprintf(stdout, "Please write in the format: './acoder M NAME KEY'\nM - operating mode:\t'e' or 'E' for encoding\n\t\t\t'd' or 'D' for decoding\nNAME - name of the file to be encoded\nKEY - encoding key: between -2147483648 and 2147483647\n\n");
		return(1);
	}

	mode=argv[1][0];
	finname=(char*)malloc(100*sizeof(char));
	for(i=0; argv[2][i]!=0; i++){
		finname[i]=argv[2][i];
	}
	finname[i]=0;


	keyn=atol(argv[3]);
	key=&keyn;

	printf("keyn: %ld\n", keyn);

	printf("key: %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd\n", key[0], key[1], key[2], key[3], key[4], key[5], key[6], key[7]);

	if(mode=='e' || mode=='E'){ //encrypt

		foutname=(char*)malloc(108*sizeof(char));

		for(i=0; finname[i]!='.'; i++){
			foutname[i]=finname[i];
		}
		for(j=0; j<12; j++, i++){
			foutname[i]=eoutname[j];
		}


		fp=fopen(finname, "r");
		fp2=fopen(foutname, "w+");

		ops=(char*)malloc(4*sizeof(char));

		for(i=0; i<4; i++){
			ops[i]=i;
		}

		while(fscanf(fp, "%c", &buff)!=EOF){

			for(i=0; i<8; i++){
				buff=appop(buff, key[i], (key[i]%4));
			}

			fprintf(fp2, "%c", buff);

			keykey=module(key[0]); //number to encrypt key
			opslc=module(key[keykey%7]) + module(((short)(key[(keykey%7)+1]))<<8); //number to select the operations
			opord=module(key[opslc%8]); //number to select the operation array order

			printf("1 keykey: %hhd\nopslc: %hd\nopord: %hhd\n\n", keykey, opslc, opord);

			for(i=0; i<4; i++){
				j=((opord>>(i*2))&3);
				buff=ops[i];
				ops[i]=ops[j];
				ops[j]=buff;
			}

			for(i=0; i<8; i++){
				j=((opslc>>(i*2))&3);
				key[i]=appop(key[i], keykey, ops[j]);
			}
			printf("2 keykey: %hhd\nopslc: %hd\nopord: %hhd\n\n", keykey, opslc, opord);

			printf("key: %hhd %hhd %hhd %hhd %hhd %hhd %hhd %hhd", key[7], key[6], key[5], key[4], key[3], key[2], key[1], key[0]);

			printf("current: %ld\n\n", keyn);
		}
	}
	else if(mode=='d' || mode=='D'){ //decrypt
		foutname=(char*)malloc(109*sizeof(char));

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
	//free(foutname);

	return 0;
}
