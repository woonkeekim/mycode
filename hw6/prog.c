#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "tablecheck.h"
#include "modprod.h"
#include "keyexpand.h"
#include "encrypt.h"
#include "decrypt.h"
#include "inverse.h"

void printUsage()
{
	printf("Usage: \n");
	printf("hw6 tablecheck -t=tablefile\n");
	printf("hw6 modprod -p1=poly1 -p2=poly2\n");
	printf("hw6 keyexpand -k=key -t=tablefile\n");
	printf("hw6 encrypt -k=key -t=tablefile [file]\n");
	printf("hw6 decrypt -k=key -t=tablefile [file]\n");
	printf("hw6 inverse -p=poly\n");
	exit(0);
}
int main(int argc, char ** argv)
{
	if (argc < 3)
		printUsage();
	if(strcmp(argv[1], "tablecheck") == 0)
	{
		if(argc != 3)
			printUsage();
		char t[strlen(argv[2]) - 3];
		int i = 0;
		if(strncmp(argv[2], "-t=", 3) == 0)
		{
			for(i = 0; i < strlen(argv[2])-3; i++)
			{
				t[i] = argv[2][i+3];
			}
			t[i] = '\0';
		}
		else
			printUsage();
		//use t
		tablecheck(t);
	}
	else if(strcmp(argv[1], "modprod") == 0)
	{
		int p1flag = 0;
		int p2flag = 0;
		int i = 0;
		char *p1;
		char *p2;
		if(argc != 4)
			printUsage();
		for(i = 2; i < 4; i++)
		{
			if(strncmp(argv[i], "-p1=", 4) == 0)
			{
				p1flag++;
				if(p1flag != 1)
					printUsage();
				p1 = (char *) malloc((strlen(argv[i]) - 4) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) -4; j++)
				{
					p1[j] = argv[i][j+4];
				}
				p1[j] = '\0';
			}
			else if(strncmp(argv[i], "-p2=", 4) == 0)
			{
				p2flag++;
				if(p2flag != 1)
					printUsage();
				p2 = (char *) malloc((strlen(argv[i]) - 4) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) -4; j++)
				{
					p2[j] = argv[i][j+4];
				}
				p2[j] = '\0';
			}
			else printUsage();
		}
		//use p1 and p2
		if(strlen(p1) != 8 || strlen(p2) != 8)
		{
			fprintf(stderr, "p1 and p2 must be length of 8\n");
			exit(0);
		}
		modprod(p1, p2);
	}
	else if(strcmp(argv[1], "keyexpand") == 0)
	{
		int kflag = 0;
		int tflag = 0;
		int i = 0;
		char *k;
		char *t;
		if(argc != 4)
			printUsage();
		for(i = 2; i < 4; i++)
		{
			if(strncmp(argv[i], "-k=", 3) == 0)
			{
				kflag++;
				if(kflag != 1)
					printUsage();
				k = (char *) malloc((strlen(argv[i]) - 3) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) -3; j++)
				{
					k[j] = argv[i][j+3];
				}
				k[j] = '\0';
			}
			else if(strncmp(argv[i], "-t=", 3) == 0)
			{
				tflag++;
				if(tflag != 1)
					printUsage();
				t = (char *) malloc((strlen(argv[i]) - 3) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) -3; j++)
				{
					t[j] = argv[i][j+3];
				}
				t[j] = '\0';
			}
			else printUsage();
		}
		//use k and t
		keyexpand(k, t);
		
	}
	else if(strcmp(argv[1], "encrypt") == 0)
	{
		int kflag = 0;
		int tflag = 0;
		int i = 0;
		char *k;
		char *t;
		if(argc < 4)
			printUsage();
		for(i = 2; i < 4; i++)
		{
			if(strncmp(argv[i], "-k=", 3) == 0)
			{
				kflag++;
				if(kflag != 1)
					printUsage();
				k = (char *) malloc((strlen(argv[i]) - 3) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) -3; j++)
				{
					k[j] = argv[i][j+3];
				}
				k[j] = '\0';
			}
			else if(strncmp(argv[i], "-t=", 3) == 0)
			{
				tflag++;
				if(tflag != 1)
					printUsage();
				t = (char *) malloc((strlen(argv[i]) - 3) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) - 3; j++)
				{
					t[j] = argv[i][j+3];
				}
				t[j] = '\0';
			}
			else printUsage();
		}
		if(argc == 4)
		{
			//use k and t
			encryptFromKey(k, t);
		}
		else if(argc == 5)
		{
			//use k and t and argv[4]
			encryptFromFile(k, t, argv[4]);			
		}
	}
	else if(strcmp(argv[1], "decrypt") == 0)
	{
		int kflag = 0;
		int tflag = 0;
		int i = 0;
		char *k;
		char *t;
		if(argc < 4)
			printUsage();
		for(i = 2; i < 4; i++)
		{
			if(strncmp(argv[i], "-k=", 3) == 0)
			{
				kflag++;
				if(kflag != 1)
					printUsage();
				k = (char *) malloc((strlen(argv[i]) - 3) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) - 3; j++)
				{
					k[j] = argv[i][j+3];
				}
				k[j] = '\0';
			}
			else if(strncmp(argv[i], "-t=", 3) == 0)
			{
				tflag++;
				if(tflag != 1)
					printUsage();
				t = (char *) malloc((strlen(argv[i]) - 3) * sizeof(char*));
				int j = 0;
				for(j = 0; j < strlen(argv[i]) - 3; j++)
				{
					t[j] = argv[i][j+3];
				}
				t[j] = '\0';
			}
			else printUsage();
		}
		if(argc == 4)
		{
			//use k and t
			decryptFromKey(k, t);
		}
		else if(argc == 5)
		{
			//use k and t and argv[4]
			decryptFromFile(k, t, argv[4]);
		}
	}
	else if(strcmp(argv[1], "inverse") == 0)
	{
		if(argc != 3)
			printUsage();
		char p[strlen(argv[2]) - 3];
		int i = 0;
		if(strncmp(argv[2], "-p=", 3) == 0)
		{
			for(i = 0; i < strlen(argv[2])-3; i++)
			{
				p[i] = argv[2][i+3];
			}
			p[i] = '\0';
		}
		else
			printUsage();
		//use p
		inverse(p);
	}
	else
		printUsage();
	return 0;
}
