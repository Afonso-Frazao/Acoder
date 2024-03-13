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

char appop(char num, char key, char op){ //apply operation
	switch(op){
		case 0:
			num+=key;
			break;
		case 1:
			num-=key;
			break;
		case 2:
			num^=key;
			break;
		case 3:
			num=(num<<(key&7)) | ((num>>((sizeof(char)*8)-(key&7)))&((1<<(key&7))-1));
			if((key&8)==8){
				num=~num;
			}
			break;
	}

	return num;
}

char unppop(char num, char key, char op){ //apply reverse operation
	switch(op){
		case 0:
			num-=key;
			break;
		case 1:
			num+=key;
			break;
		case 2:
			num^=key;
			break;
		case 3:
			num=(num<<((sizeof(char)*8)-(key&7))) | ((num>>(key&7))&((1<<((sizeof(char)*8)-(key&7)))-1));
			if((key&8)==8){
				num=~num;
			}
			break;
	}

	return num;
}

void encryptkey(char *key, char *ops){
	char buff, keykey, opord;
	short opslc;
	int i, j;

	for(i=0; i<8; i++){
        keykey=module(key[i]); //number to encrypt key
        if(keykey!=0){
			break;
        }
    }
    if(keykey==0){
    	keykey=85;
    }
    opslc=module(key[keykey%7]) + module(((short)(key[(keykey%7)+1]))<<8); //number to select the operations
    opord=module(key[opslc%8]); //number to select the operation array order

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

	return;
}

int main(int argc, char **argv){
	char mode, fnsize;
	int i, j, fncount; //file name count
	long keyn;
	char *finname, *dename, *key;
	char keykey, opord;
	short opslc;
	char *ops;
	char foutname[11]="acoded.txt";
	//char eoutname[12]="_acoded.txt";
	//char doutname[13]="_decoded.txt";
	char buff;
	FILE *fp, *fp2;

	if(argc!=4){
		fprintf(stdout, "Usage: './acoder M NAME KEY'\nM - operating mode:\t'e' or 'E' for encoding\n\t\t\t'd' or 'D' for decoding\nNAME - name of the file to be encoded\nKEY - encoding key: between -9.223.372.036.854.775.808 and 9.223.372.036.854.775.808\n\n");
		return(1);
	}

	mode=argv[1][0];
	finname=(char*)malloc(100*sizeof(char));
	for(fncount=0; argv[2][fncount]!=0; fncount++){
		finname[fncount]=argv[2][fncount];
	}
	finname[fncount]=0;
	printf("fncount: %d\n", fncount);

	keyn=atol(argv[3]);
	key=&keyn;

	ops=(char*)malloc(4*sizeof(char));

	for(i=0; i<4; i++){
    	ops[i]=i;
    }

	if(mode=='e' || mode=='E'){ //encrypt

		fp=fopen(finname, "rb");
		fp2=fopen(foutname, "w+");

		buff=(char)fncount;
		for(i=0; i<8; i++){ //write file name size
            buff=appop(buff, key[i], ops[module(key[i]%4)]);
        }
		encryptkey(key, ops);
		printf("acoded: %hhd\n", buff);
		fprintf(fp2, "%c", buff);
		
		for(i=0; i<fncount; i++){ //write file name
			buff=finname[i];
			for(j=0; j<8; j++){
				buff=appop(buff, key[j], ops[module(key[j]%4)]);
			}
			fprintf(fp2, "%c", buff);
			encryptkey(key, ops);
		}

		while(fscanf(fp, "%c", &buff)!=EOF){
			
			for(i=0; i<8; i++){
				buff=appop(buff, key[i], ops[module(key[i]%4)]);
			}

			fprintf(fp2, "%c", buff);

			if(key[7]<-64){ //create trash information/hide the file name
				buff=key[6];
				for(i=0; i<8; i++){
                	buff=appop(buff, key[i], ops[module(key[i]%4)]);
            	}
				fprintf(fp2, "%c", buff);
			}

			encryptkey(key, ops);
		}

	}
	else if(mode=='d' || mode=='D'){ //decrypt

		fp=fopen(finname, "rb");

		fscanf(fp, "%c", &fnsize);
		printf("received: %hhd\n", fnsize);
		for(i=7; i>=0; i--){
			fnsize=unppop(fnsize, key[i], ops[module(key[i]%4)]);
		}
		encryptkey(key, ops);

		printf("decoded: %hhd\n", fnsize);

		dename=(char*)malloc((fnsize+1)*sizeof(char));

		for(i=0; i<fnsize; i++){
			fscanf(fp, "%c", &buff);
			for(j=7; j>=0; j--){
				//printf("%hhd\n", key[j]);
				//printf("%hhd\n", ops[module(key[j]%4)]);
				buff=unppop(buff, key[j], ops[module(key[j]%4)]);
			}
			encryptkey(key, ops);
			dename[i]=buff;
			printf("\t%c\n", buff);
		}
		dename[fnsize]=0;

		fp2=fopen(dename, "w+");

		while(fscanf(fp, "%c", &buff)!=EOF){

			for(i=7; i>=0; i--){
                buff=unppop(buff, key[i], ops[module(key[i]%4)]);
            }

            fprintf(fp2, "%c", buff);

			if(key[7]<-64){ //discard trash information
				fseek(fp, 1, SEEK_CUR);
			}

            encryptkey(key, ops);
		}

	}
	else{
		return(1);
	}

	fclose(fp);
	fclose(fp2);

	free(finname);

	return 0;
}
