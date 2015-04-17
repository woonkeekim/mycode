#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablecheck.h"
#include "encrypt.h"
#include "encrypt3.h"
#include "decrypt.h"
void printUsage()
{
	printf("Usage: \n");
	printf("hw5 tablecheck -t=tablefile\n");
	printf("hw5 encrypt -k=key -t=tablefile [file]\n");
	printf("hw5 decrypt -k=key -t=tablefile [file]\n");
	printf("hw5 encrypt3 -k=key3 -t=tablefile [file]\n");
	printf("hw5 decrypt3 -k=key3 -t=tablefile [file]\n");
	printf("[file] is optional\n");
	exit(0);
}

int main(int argc, char ** argv) 
{	
	if(argc < 3)
		printUsage();	
	else
	{
		if(strcmp("tablecheck", argv[1]) == 0)
		{
			char *tablefile;
			if(argc != 3)
				printUsage();
			if(strncmp("-t=", argv[2], 3) == 0)
			{
				tablefile = (char *) malloc((strlen(argv[2]) -3) * sizeof(char*));
				tablefile = argv[2]+3;
				tablecheck(tablefile);
			}
			else
				printUsage();
			//use tablefile
		}
		else if(strcmp("encrypt", argv[1]) == 0)
		{
			char *key;
			char *tablefile;
			int keyFlag = 0;
			int tableFlag = 0;
			int i = 0;
			if(argc < 4 || argc > 5)
				printUsage();
			for(i = 2; i < 4; i++)
			{
				if(strncmp("-k=", argv[i], 3) == 0)
				{
					if(keyFlag > 0)
						printUsage();
					keyFlag++;
					key = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					key = argv[i]+3;
				}
				else if(strncmp("-t=", argv[i], 3) == 0)
				{
					if(tableFlag > 0)
						printUsage();
					keyFlag++;
					tablefile = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					tablefile = argv[i]+3;
				}
				else
					printUsage();
			}
			//use key, tablefile
			if(argc == 4)
			{
				//read from keyboard
				encryptFromKey(key, tablefile);
			}
			else
			{
				//read from file
				encrypt(key, tablefile, argv[4]);
			}
		}
		else if(strcmp("decrypt", argv[1]) == 0)
		{
			char *key;
			char *tablefile;
			int keyFlag = 0;
			int tableFlag = 0;
			int i = 0;
			if(argc < 4 || argc > 5)
				printUsage();
			for(i = 2; i < 4; i++)
			{
				if(strncmp("-k=", argv[i], 3) == 0)
				{
					if(keyFlag > 0)
						printUsage();
					keyFlag++;
					key = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					key = argv[i]+3;
				}
				else if(strncmp("-t=", argv[i], 3) == 0)
				{
					if(tableFlag > 0)
						printUsage();
					keyFlag++;
					tablefile = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					tablefile = argv[i]+3;
				}
				else
					printUsage();
			}
			//use key, tablefile
			if(argc == 4)
			{
				//read from keyboard
				 decryptFromKey(key, tablefile);
			}
			else
			{
				//read from file
				 decrypt(key, tablefile, argv[4]);
			}
		}
		else if(strcmp("encrypt3", argv[1]) == 0)
		{
			char *key;
			char *tablefile;
			int keyFlag = 0;
			int tableFlag = 0;
			int i = 0;
			if(argc < 4 || argc > 5)
				printUsage();
			for(i = 2; i < 4; i++)
			{
				if(strncmp("-k=", argv[i], 3) == 0)
				{
					if(keyFlag > 0)
						printUsage();
					keyFlag++;
					key = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					key = argv[i]+3;
				}
				else if(strncmp("-t=", argv[i], 3) == 0)
				{
					if(tableFlag > 0)
						printUsage();
					keyFlag++;
					tablefile = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					tablefile = argv[i]+3;
				}
				else
					printUsage();
			}
			//use key, tablefile
			if(argc == 4)
			{
				//read from keyboard
				 encrypt3FromKey(key, tablefile);
			}
			else
			{
				//read from file
				 encrypt3(key, tablefile, argv[4]);
			}
		}
		else if(strcmp("decrypt3", argv[1]) == 0)
		{
			char *key;
			char *tablefile;
			int keyFlag = 0;
			int tableFlag = 0;
			int i = 0;
			if(argc < 4 || argc > 5)
				printUsage();
			for(i = 2; i < 4; i++)
			{
				if(strncmp("-k=", argv[i], 3) == 0)
				{
					if(keyFlag > 0)
						printUsage();
					keyFlag++;
					key = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					key = argv[i]+3;
				}
				else if(strncmp("-t=", argv[i], 3) == 0)
				{
					if(tableFlag > 0)
						printUsage();
					keyFlag++;
					tablefile = (char *) malloc((strlen(argv[i])-3) * sizeof(char*));
					tablefile = argv[i]+3;
				}
				else
					printUsage();
			}
			//use key, tablefile
			if(argc == 4)
			{
				//read from keyboard
			}
			else
			{
				//read from file
			}
		}
		else
			printUsage();
	}
	return 0;
}
