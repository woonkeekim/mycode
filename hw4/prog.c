#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "keygen.h"
#include "crypt.h"
#include "invkey.h"
#include "histo.h"
#include "solve.h"
void printUsage()
{
	printf("The usage of hw4:\n");
	printf("	hw4 keygen -p=pphrase -t=period\n");
	printf("	hw4 crypt -k=keyfile [file]\n");
	printf("	hw4 invkey keyfile\n");
	printf("	hw4 histo -t=period -i=which [file]\n");
	printf("	hw4 solve -l=max_t file\n");
	printf("	[file] is optional\n");
}
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


int main(int argc, char **argv)
{
	if(argc < 2)
	{
		printUsage();
		exit(0);
	}
	else
	{
		if(strcmp(argv[1], "keygen") ==0)
		{
			int pflag = 0;
			int tflag = 0;
			if(argc != 4)
			{
				printUsage();
				exit(0);
			}
			int i = 0;
			char *pValue;
			int tValue = 0;
			char *tValue_tmp;
			for(i = 2; i < 4; i++)
			{
				if(strncmp(argv[i], "-p=", 3) == 0)
				{
					pflag++;
					if(pflag > 1)
					{
						printf("You declared p value more than one time\n");
						exit(0);
					}
					pValue = (char *)malloc(sizeof(char*) * (strlen(argv[i])-3));
					int j = 0;
					int k = 0;
					for(j = 3; j < strlen(argv[i]); j++)
					{
						pValue[k] = argv[i][j];
						k++;
					}
					if(k == 0)
					{
						printf("You must enter p Value\n");
						exit(0);
					}
					pValue[k] = '\0';
				}
				else if(strncmp(argv[i], "-t=", 3) == 0)
				{
					tflag++;
					if(tflag > 1)
					{
						printf("You declared p value more than one time\n");
						exit(0);
					}
					tValue_tmp = (char *)malloc(sizeof(char *) * (strlen(argv[i])-3));
					int j = 0;
					int k = 0;
					for(j = 3; j < strlen(argv[i]); j++)
					{
						tValue_tmp[k] = argv[i][j];
						k++;
					}
					if(k == 0)
					{
						printf("You must enter t Value\n");	
						exit(0);
					}
					tValue_tmp[k] = '\0';
					if(!isDigit(tValue_tmp))
					{
						printf("You must enter t value as integer\n");
						exit(0);
					}
					tValue = atoi(tValue_tmp);
				}
				else
				{
					printUsage();
					exit(0);
				}
			}
			//use (char *) pValue, int tValue)
			keygen(pValue, tValue);
		}
		else if(strcmp(argv[1], "crypt") == 0)
		{
			if(argc < 3 || argc > 4)
			{
				printUsage();
				exit(0);
			}
			char *kValue;
			if(strncmp(argv[2], "-k=", 3) == 0)
			{
				kValue = (char *) malloc(sizeof(char*) * (strlen(argv[2])-3));
				int j = 0;
				int k = 0;
				for(j = 3; j < strlen(argv[2]); j++)
				{
					kValue[k] = argv[2][j];
					k++;
				}
				if(k==0)
				{
					printf("You must enter k Value\n");
					exit(0);
				}
				kValue[k] = '\0';
			}
			else
			{
				printUsage();
				exit(0);
			}
			if(argc == 3)
			{
				//use (char *)  kValue
				cryptFromKey(kValue);
			}
			else if(argc == 4)
			{
				// use (char *) kValue, file as argv[3]
				crypt(kValue, argv[3]);
			}
			else
			{
				printUsage();
				exit(0);
			}
		}
		else if(strcmp(argv[1], "invkey") == 0)
		{
			if(argc != 3)
			{
				printUsage();
				exit(0);
			}
			// do arg[2]
			invkey(argv[2]);

		}
		else if(strcmp(argv[1], "histo") == 0)
		{
			if(argc < 4 || argc > 5)
			{
				printUsage();
				exit(0);
			}
			int i = 0;
			int tflag = 0;
			int iflag = 0;
			char *tValue_tmp;
			char *iValue_tmp;
			int tValue;
			int iValue;
			for(i = 2; i < 4; i++)
			{
				if(strncmp(argv[i], "-t=", 3) == 0)
				{
					tflag++;
					if(tflag > 1)
					{
						printf("You declared t value more than one time");
					}
					int j = 0;
					int k = 0;
					tValue_tmp = (char *) malloc(sizeof(char*) * (strlen(argv[i])-3));
					for(j = 3; j < strlen(argv[i]); j++)
					{
						tValue_tmp[k] = argv[i][j];
						k++;
					}
					if(k==0)
					{
						printf("You must declare t value\n");
						exit(0);
					}
					tValue_tmp[k] = '\0';
					if(!isDigit(tValue_tmp))
					{
						printf("t value must be integer\n");
						exit(0);
					}
					tValue = atoi(tValue_tmp);
				}
				else if(strncmp(argv[i], "-i=", 3) == 0)
				{
					iflag++;
					if(iflag > 1)
					{
						printf("You declared i value more than one time");
					}
					int j = 0;
					int k = 0;
					iValue_tmp = (char *) malloc(sizeof(char*) * (strlen(argv[i])-3));
					for(j = 3; j < strlen(argv[i]); j++)
					{
						iValue_tmp[k] = argv[i][j];
						k++;
					}
					if(k==0)
					{
						printf("You must declare i value\n");
						exit(0);
					}
					iValue_tmp[k] = '\0';
					if(!isDigit(iValue_tmp))
					{
						printf("i value must be integer\n");
						exit(0);
					}
					iValue = atoi(iValue_tmp);
				}
				else
				{
					printUsage();
					exit(0);
				}
			}
			if(argc	== 4)
			{
				//use tValue, iValue
				histoFromKey(tValue, iValue);
			}
			else if(argc == 5)
			{
				//use tValue, iValue, argv[4]
				histo(tValue, iValue, argv[4]);
			}
			else
			{
				printUsage();	
				exit(0);
			}
		}
		else if(strcmp(argv[1], "solve") == 0)
		{
			if(argc != 4)
			{
				printUsage();
				exit(0);
			}
			char *lValue_tmp;
			int lValue;
			if(strncmp(argv[2], "-l=", 3) == 0)
			{
				int j = 0;
				int k = 0;
				lValue_tmp = (char *) malloc(sizeof(char *) * (strlen(argv[2])-3));
				for(j = 3; j < strlen(argv[2]); j++)
				{
					lValue_tmp[k] = argv[2][j];
					k++;
				}
				if(k == 0)
				{
					printf("You must declare l value\n");
					exit(0);
				}
				lValue_tmp[k] = '\0';
				if(!isDigit(lValue_tmp))
				{
					printf("l value must be integer\n");
					exit(0);
				}
				lValue = atoi(lValue_tmp);
			}
			else
			{
				printUsage();
				exit(0);
			}
			solve(lValue, argv[3]);
		}
		else
		{
			printUsage();	
			exit(0);
		}

	}
	return 0;
}
