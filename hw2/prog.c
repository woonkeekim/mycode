#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stream.h"
#include "encrypt.h"
#include "merge.h"
#include "decrypt.h"

void printUsage()
{
	printf("hw2 stream -p=pphrase -l=len\n");
	printf("hw2 encrypt -p=pphrase -out=name [pbmfile]\n");
	printf("hw2 merge pbmfile1 pbmfile2\n");
	printf("hw2 decrypt [pbmfile]\n");
	printf("   [pbmfile] is optional\n");
}
/*
int isDigit(char *input)
{
	int i =0;
	for(i = 0; i < strlen(input); i++)
	{
		if((input[i] - '0') > 9 || (input[i] - '0') < 0)
		{	
			return 0;
		}
	}
	return 1;
}
*/
int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printUsage();
		exit(0);
	}
	else if(argc >= 2)
	{
		if(strcmp(argv[1], "stream") == 0)
		{
			int i = 3;
			char *pphrase;
			int length = 0;
			int pFlag = 0;
			int lFlag = 0;
			if(argc != 4)
			{
				printUsage();
				exit(0);
			}
			for(i = 2; i < 4; i++)
			{
				if(strncmp(argv[i], "-p=", 3) == 0)
				{
					pFlag++;
					if(pFlag > 1)
					{
						printf("You need to specify length\n");
						exit(0);
					}
					int j = 0;
					int k = 0;
					pphrase = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					for(k = 3; k < strlen(argv[i]); k++)
					{
						pphrase[j] = argv[i][k];
						j++;
					}
					pphrase[j] = '\0';
					if(j == 0)
					{
						printf("You must enter pass phrase\n");
						exit(0);
					}
				}
				else if(strncmp(argv[i], "-l=", 3) == 0)
				{
					lFlag++;
					if(lFlag > 1)
					{
						printf("You need to specify pass phrase\n");
						exit(0);
					}
					
					int k = 0;
					int j = 0;
					char *tmp;
					tmp = (char *) malloc(strlen(argv[i]-3));
					for(k = 3; k < strlen(argv[i]); k++)
					{
						tmp[j] = argv[i][k];
						j++;
					}
					if(j == 0)
					{
						printf("You must enter Length value\n");
						exit(0);
					}
					tmp[j] = '\0';
				
					if(!isDigit(tmp))
					{
						printf("Length must be a number\n");
						free(tmp);
						exit(0);
					}
					else
					{
						length = atoi(tmp);
					}
				}
				else
				{
					printUsage();
					exit(0);
				}
			}
			//Call stream with argument length and pphrase;
                        unsigned char result[length];
                        stream(pphrase, length, result);   
                        for(i = 0; i < length; i++) 
                        {
                                printf("%c", result[i]);
                        }

		}
		//hw2 encrypt -p=pphrase -out=name [pbmfile]
		else if(strcmp(argv[1], "encrypt") == 0)
		{
			int i = 0;
			char *pphrase;
			char *outputName[2];
			int pFlag = 0;
			int outFlag = 0;
			if(argc < 4 || argc > 5)
			{
				printUsage();
				exit(0);
			}
			for(i = 2; i < 4; i++)
			{
				if(strncmp(argv[i], "-p=", 3) == 0)
				{
					pFlag++;
					if(pFlag > 1)
					{
						printf("You need to specify length\n");
						exit(0);
					}
					int j = 0;
					int k = 0;
					pphrase = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					for(k = 3; k < strlen(argv[i]); k++)
					{
						pphrase[j] = argv[i][k];
						j++;
					}
					pphrase[j] = '\0';
					if(j == 0)
					{
						printf("You must enter pass phrase\n");
						exit(0);
					}
				}
				else if(strncmp(argv[i], "-out=", 5) == 0)
				{
					outFlag++;
					if(outFlag > 1)
					{
						printf("You need to specify passphrase\n");
						exit(0);
					}
					int j = 0;
					int k = 0;
					char tmp[strlen(argv[i])-5];
					
					for(k= 5; k < strlen(argv[i]); k++)
					{
						tmp[j] = argv[i][k];
						j++;
					}
					if(j == 0)
					{
						printf("You must enter output file name\n");
						exit(0);
					}
					tmp[j] = '\0';
					outputName[0] = (char *) malloc(strlen(argv[i])+1 * sizeof(char));
					outputName[1] = (char *) malloc(strlen(argv[i])+1 * sizeof(char));
					strcat(outputName[0], tmp);
					strcat(outputName[0], ".1.pbm");
					
					strcat(outputName[1], tmp);
					strcat(outputName[1], ".2.pbm");		
				}
				else
				{
					printUsage();
					exit(0);
				}

			}
			if(argc == 5)
			{
				encrypt(pphrase, outputName[0], outputName[1], argv[4]);
			}
			else if(argc == 4)
			{
				encryptKey(pphrase, outputName[0], outputName[1]);
			}
			else
			{
				printUsage();
				exit(0);
			}
		}
		//"hw2 merge pbmfile1 pbmfile2\n"
		else if(strcmp(argv[1], "merge") == 0)
		{
			if(argc != 4)
			{
				printUsage();
				exit(0);
			}
			//printf("file 1 is %s\n", argv[2]);
			//printf("file 2 is %s\n", argv[3]);
			merge(argv[2], argv[3]);
		}
		//hw2 decrypt [pbmfile]\n"
		else if(strcmp(argv[1], "decrypt") == 0)
		{
			if(argc == 3)
			{
				//printf("file name is %s", argv[2]);
				decrypt(argv[2]);
			}
			else if(argc == 2)
			{
				decryptKey();
			}
			else
			{
				printUsage();
				exit(0);
			}
		}
		else
		{
			printUsage();
			exit(0);
		}
	}
 return 0;
}

