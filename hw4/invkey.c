#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

void invkey(char *input)
{
	FILE *fp;
	fp = fopen(input, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "File %s cannot be opened\n", input);
		exit(0);
	}

	char *state = "abcdefghijklmnopqrstuvwxyz";
	int size = 0;
	char *buf;
	int charCounter = 0;
	buf = (char *) malloc(sizeof(char*));
	char *key;
	key = (char *) malloc(sizeof(char*) * 26);
	while((size = fread(buf, 1, 1, fp)) != 0)
	{
		if(charCounter != 26)
		{
			if(buf[0] < 'a' || buf[0] > 'z')
			{
				printf("Key file is a wrong format\n");
				fclose(fp);
				exit(0);
			}

			key[charCounter] = buf[0];
			charCounter++;
		}
		else
		{

			int i = 0;
			int j = 0;
			for(i = 0; i < 26; i++)
			{
				for(j = 0; j < 26; j++)
				{
					if((key[j] - 'a') == i)
					{
						printf("%c", state[j]);
						break;
					}
				}
			}
			key = (char *) malloc(sizeof(char*) * 26);
			charCounter = 0;
			printf("\n");
		}
	}
	
}
