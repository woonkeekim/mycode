#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
int CorrectPattern(char * input)
{
	int i = 0;
	for(i = 0; i < strlen(input); i++)
	{
		if(input[i] < 'a' || input[i] > 'z')
			return 0;
	}
	return 1;
}
void solve(int lValue, char *input)
{
	FILE *fp;
	fp = fopen(input, "r");
	if(fp == NULL)
	{
		printf("The file %s does not exist\n", input);
		exit(0);
	}
	fseek(fp, 0, SEEK_END);
	int byteCount = ftell(fp);
	rewind(fp);
	char *buf;
	buf = (char *) malloc(sizeof(char *));
	char *file;
	file = (char *) malloc(sizeof(char*) * byteCount);
	int size = 0;
	int fileCounter = 0;
	int ICTable[26];
	int ICTotal = 0;
	int j = 0;
	for(j = 0; j < 26; j++)
	{
		ICTable[j]=0;
	}
	while((size = fread(buf,1,1,fp)) != 0)
	{
		file[fileCounter] = buf[0];
		fileCounter++;
		if(buf[0] >= 'a' && buf[0] <= 'z')
		{
			ICTable[buf[0]-'a']++;
			ICTotal++;
		}
	}
	file[fileCounter] = '\0';
	fclose(fp);


	//try to print all 4 length chars
	int i = 4;
	char *pattern;
	int k = 0;
	int available = 0;
	printf("Kasiski Method\n");
	printf("==============\n");
	for(i = 4; ; i++)
	{
		available = 0;
		for(k = 0; k < strlen(file); k++)
		{
			pattern = (char *) malloc(sizeof(char *)*i);
			int j = 0;
			for(j = 0; j < i; j++)	
			{
				pattern[j] = file[k+j];	
			}
			pattern[j] = '\0';
			if(!CorrectPattern(pattern))
				continue;
			else
			{
				char* ptr;
				ptr = strstr((file+k+1), pattern); 
				while(ptr != NULL)
				{
					printf("len=%d, i=%d, j=%d, j-i=%d, %s\n", i, k, (ptr-file), (ptr-file)-k, pattern);
					ptr = strstr(file+(ptr-file)+1, pattern);
					available++;
				}
			}
		}
		if(available == 0)
		{
			printf("len=%d, no more matches\n", i);
			break;
		}
	}
	printf("\n");
	printf("Average Index of Coincidence\n");
	printf("============================\n");
	printf("L=%d\n", ICTotal);
	for(i = 0; i < 26; i++)
	{
		printf("f('%c')=%d\n", i+'a' , ICTable[i]);
	}
	double IC = 0;
	for(i = 0; i < 26; i++)
	{
		IC = IC+(ICTable[i]*(ICTable[i]-1));
	}
	IC = IC/(ICTotal*(ICTotal-1));
	printf("IC=%.9f\n", IC);
	double kp = 0.0658;
	double kr = (double)1/26;

	for(i = 1; i <= lValue; i++)
	{

		double top = (double)(ICTotal-i)*kp;
		double bottom = (double)(i*(ICTotal-1));
		double top1 = (double)(i-1)*ICTotal*kr;
		double bottom1 = (double)i*(ICTotal-1);

		printf("t=%d, E(IC)=%0.8g\n", i, (double)top/bottom+(double)top1/bottom1);
	}
	printf("\n");
	printf("Auto-correlation Method\n");
	printf("=======================\n");	
	j = 0;
	int count = 0;
	for(i = 1; i <= lValue; i++)
	{
		count = 0;
		for(j = 0; j < strlen(file)-i; j++)
		{
			if(file[j] == file[j+i])
			{
				if(file[j] >= 'a' && file[j] <='z')
					count++;
			}
		}
		printf("t=%d, count=%d\n", i, count);
	}
	printf("\n");
}
