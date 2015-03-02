#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
void cryptFromKey(char *keyFile)
{
        FILE * fp1;
        fp1 = fopen(keyFile, "r");
        if(fp1 == NULL)
        {
                printf("The file %s cannot be opened\n", keyFile);
                exit(0);
        }
        //creating keytable
        int keyLen = 1;
        char **keys;
        int size = 0; 
        keys = (char **) malloc(keyLen * sizeof(char**));
        keys[0] = (char *) malloc( 26 * sizeof(char*));
                
        char *buf;
        int rowCounter = 0;
        buf = (char *) malloc(sizeof(char*));
        while((size = fread(buf, 1, 1, fp1)) != 0)
        {
                if(buf[0] == '\n')
                {
                        continue;
                }
                if(rowCounter == 26)
                {
                        rowCounter = 0;
                        keyLen++;
                        keys = (char **) realloc(keys, keyLen * sizeof(char*));
                        keys[keyLen-1] = (char *) malloc(26*sizeof(char *));
                }
		if(buf[0] < 'a' || buf[0] > 'z')
		{
			printf("Key file is a wrong format\n");
			fclose(fp1);
			exit(0);
		}
                keys[keyLen-1][rowCounter] = buf[0];
                rowCounter++;
        
        }
        fclose(fp1);
         
        int charCounter = 0;
        while((size = fread(buf, 1, 1, stdin)) != 0)
        {
                if(buf[0] - 'a' >= 0 && buf[0] <= 'z')
                {
                        printf("%c", keys[charCounter % keyLen][buf[0] - 'a']);
                        charCounter++; 
                }
                else
                {
                        printf("%c", buf[0]);
                        charCounter++;
                }
        }

}

void crypt(char * keyFile, char * input)
{
	FILE * fp1;
	fp1 = fopen(keyFile, "r");
	if(fp1 == NULL)
	{
		printf("The file %s cannot be opened\n", keyFile);
		exit(0);
	}


	//creating keytable
	int keyLen = 1;
	char **keys;
	int size = 0;
	keys = (char **) malloc(keyLen * sizeof(char**));
	keys[0] = (char *) malloc( 26 * sizeof(char*));

	char *buf;
	int rowCounter = 0;
	buf = (char *) malloc(sizeof(char*));
	while((size = fread(buf, 1, 1, fp1)) != 0)
	{
		if(buf[0] == '\n')
		{
			continue;
		}
		if(rowCounter == 26)
		{
			rowCounter = 0;
			keyLen++;
			keys = (char **) realloc(keys, keyLen * sizeof(char*)); 
			keys[keyLen-1] = (char *) malloc(26*sizeof(char *));
		}
		if(buf[0] < 'a' || buf[0] > 'z')
		{
			printf("Key file is a wrong format\n");
			fclose(fp1);
			exit(0);
		}
		keys[keyLen-1][rowCounter] = buf[0];
		rowCounter++;

	}
	fclose(fp1);
	FILE * fp;
	fp = fopen(input, "r");
	if(fp == NULL)
	{
		printf("The file %s cannot be opened\n", input);
		exit(0);
	}

	int charCounter = 0;
	while((size = fread(buf, 1, 1, fp)) != 0)
	{
		if(buf[0] - 'a' >= 0 && buf[0] <= 'z')
		{
			printf("%c", keys[charCounter % keyLen][buf[0] - 'a']);
			charCounter++;
		}
		else
		{
			printf("%c", buf[0]);
			charCounter++;
		}
	}
	fclose(fp);

}
