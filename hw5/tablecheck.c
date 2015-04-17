#include <stdio.h>
#include <stdlib.h>
#include <string.h>
int isDigit(char *input)
{
	int i =0;
	for(i = 0; i < strlen(input); i++)
	{
		if(input[i] < 48 || input[i] > 57)
			return 0;
	}
	return 1;
}

void printError()
{
	printf("the file is wrong format\n");
	exit(0);
}

void tablecheck(char *file)
{
	FILE *fp = fopen(file, "r");
	if(fp == NULL)
	{
		printf("the input file %s does not exist\n", file);
		exit(0);
	}
	char header[4];
	char *buf;
	buf = (char *) malloc(sizeof(char*));
	int size = 0;
	int i = 0;
	int headerPoint = 0;
	int IPFlag = 0;
	int EFlag = 0;
	int PFlag = 0;
	int SFlag[8];
	for(i = 0; i < 8; i++)
		SFlag[i] = 0;
	int VFlag = 0;
	int PC1Flag = 0;
	int PC2Flag = 0;

	for(i = 0; i < 14; i++)
	{
		if((size = fread(buf, 1, 1, fp)) != 1)
			printError();

		while(buf[0] != '=')
		{
			header[headerPoint] = buf[0];
			headerPoint++;
			if((size = fread(buf,1,1,fp)) != 1)
				printError();
			if(headerPoint > 3)
				printError();
		}
		header[headerPoint] = '\0';
		if(strcmp(header, "IP") == 0)
		{
			IPFlag++;
			if(IPFlag != 1)
				printError();
			int t[64];
			int j = 0;
			char *value;
			value = (char *) malloc(2*sizeof(char*));
			int valueP = 0;
			for(j = 0; j < 64; j++)
				t[j] = 0;
			for(j = 0; j < 64; j++)
			{
				if((size = fread(buf,1,1,fp)) != 1)
				{
					printf("%s: ( IP) %d values read while 64 was expected\n", file, j);
					exit(0);
				}
				while(buf[0] != '\n' && buf[0] != ',')
				{
					value[valueP] = buf[0];
					if((size = fread(buf,1,1,fp)) != 1)
					{
						printf("%s: ( IP) too few values read\n", file);
						exit(0);
					}
					valueP++;
				}
				value[valueP] = '\0';
				if(!isDigit(value))
				{
					printf("%s: ( IP) at %d, the value is not correct\n", file, j +1);
					exit(0);
				}
				t[atoi(value)-1]++;
				valueP = 0;
				value[0] = '\0';
			}
			if(buf[0] != '\n')
			{
				while(buf[0] != '\n')
				{
					fread(buf,1,1,fp);
					if(buf[0] == ',' || buf[0] == '\n')
						j++;
				}
				printf("%s: ( IP) %d values read while 64 was expected\n", file, j);
				exit(0);
			}

			for(j = 0; j < 64; j++)
			{
				if(t[j] != 1)
				{
					printf("%s: ( IP) the value %d occured too many or did not occur\n", file, j);
					exit(0); 
				}
			}
		}
		else if(strcmp(header, "E") == 0)
		{
			EFlag++;
			if(EFlag != 1)
				printError();
			int t[32];
			int j = 0;
			char *value;
			value = (char *) malloc(2*sizeof(char*));
			int valueP = 0;
			for(j = 0; j < 32; j++)
				t[j] = 0;
			for(j = 0; j < 48; j++)
			{
				if((size = fread(buf,1,1,fp)) != 1)
				{
					printf("%s: (  E) %d values read while 48 was expected\n", file, j);
					exit(0);
				}

				while(buf[0] != '\n' && buf[0] != ',')
				{
					value[valueP] = buf[0];
					if((size = fread(buf,1,1,fp)) != 1)
						printError();
					valueP++;
				}
				value[valueP] = '\0';
				if(!isDigit(value))
				{
					printf("%s: ( E) at %d, the value is not correct\n", file, j+1);
					exit(0);
				}
				t[atoi(value)-1]++;
				valueP = 0;
				value[0] = '\0';
			}
			if(buf[0] != '\n')
			{
				while(buf[0] == '\n')
				{
					fread(buf,1,1,fp);
					if(buf[0] == ',' || buf[0] == '\n')
						j++;
				}
				printf("%s: (  E) %d values read while 48 was expecte\n", file, j);
				exit(0);
			}

			for(j = 0; j < 32; j++)
			{
				if(t[j] < 1 || t[j] > 2)
				{
					printf("%s: (  E) the value %d occurs too few or too many times\n", file, j);
				}
			}
			
		}
		else if(strcmp(header, "P") == 0)
		{
			PFlag++;
			if(PFlag != 1)
				printError();
			int t[32];
			int j = 0;
			char *value;
			value = (char *) malloc(2*sizeof(char*));
			int valueP = 0;
			for(j = 0; j < 32; j++)
				t[j] = 0;
			for(j = 0; j < 32; j++)
			{
				if((size = fread(buf,1,1,fp)) != 1)
				{
					printf("%s: (  P) %d values read while 32 was expected\n", file, j);
					exit(0);
				}
				while(buf[0] != '\n' && buf[0] != ',')
				{
					value[valueP] = buf[0];
					if((size = fread(buf,1,1,fp)) != 1)
						printError();
					valueP++;
				}
				value[valueP] = '\0';
				if(!isDigit(value))
				{
					printf("%s: ( P) at %d, the value is not correct\n", file, j+1);
					exit(0);
				}
				t[atoi(value)-1]++;
 				valueP = 0;
				value[0] = '\0';
			}
			if(buf[0] != '\n')
			{
				while(buf[0] != '\n')
				{
					fread(buf,1,1,fp);
					if(buf[0] == ',' || buf[0] == '\n')
						j++;
				}
				printf("%s: (  P) %d values read while 32 was expected\n", file, j);
				exit(0);
			}

			for(j = 0; j < 32; j++)
			{
				if(t[j] != 1)
				{
					printf("%s: (  P) the value %d occurs too many or too fewer times\n", file, j);
					exit(0);
				}
			}
		}
		else if(strncmp(header, "S", 1) == 0)
		{
			if(strlen(header) != 2)
				printError();
			if(header[1] < '1' || header[1] > '8')
				printError();
			SFlag[(int)header[1]-'0'-1]++;
			if(SFlag[(int)header[1]-'0'-1] != 1)
				printError();
			int t[16];
			int j = 0;
			char *value;
			value = (char *) malloc(2*sizeof(char*));
			int valueP = 0;
			int k = 0;
			for(k = 0; k < 4; k++)
			{
				for(j = 0; j < 16; j++)
					t[j] = 0;
				for(j = 0; j < 16; j++)
				{
					if((size = fread(buf,1,1,fp)) != 1)
					{
						printf("%s: ( S%d) in row %d, %d values read while 16 was expected\n", file, header[1] -'0', k, j);
						exit(0);
					}
					while(buf[0] != '\n' && buf[0] != ',')
					{
						value[valueP] = buf[0];
						if((size = fread(buf,1,1,fp)) != 1)
						{
							printf("%s: ( S%d) in row %d, %d values read while 16 was expected\n", file, header[1] -'0', k, j);
							exit(0);
						}
						valueP++;
					}
					value[valueP] = '\0';
					if(!isDigit(value))
					{
						printf("%s: ( S%d) in row %d, %d values has an error\n", file, header[1] -'0', k, j);
						exit(0);
					}
					t[atoi(value)]++;
 					valueP = 0;
					value[0] = '\0';
				}
				for(j = 0; j < 16; j++)
				{
					if(t[j] != 1)
					{
						printf("%s: ( S%d) in row %d, the values %d occur too many or too fewer times\n", file, header[1] -'0', k, j);
					}
				}
			}
		}
		else if(strcmp(header, "V") == 0)
		{
			VFlag++;
			if(VFlag != 1)
				printError();
			int t[2];
			int j = 0;
			char *value;
			value = (char *) malloc(2*sizeof(char*));
			int valueP = 0;
			for(j = 0; j < 2; j++)
				t[j] = 0;
			for(j = 0; j < 16; j++)
			{
				if((size = fread(buf,1,1,fp)) != 1)
				{
					printf("%s: (  V) %d values read while 16 was expected\n", file, j);
					exit(0);
				}
				while(buf[0] != '\n' && buf[0] != ',')
				{
					value[valueP] = buf[0];
					if((size = fread(buf,1,1,fp)) != 1)
						printError();
					valueP++;
				}
				value[valueP] = '\0';
				if(!isDigit(value))
				{
					printf("%s: (  V) at %d, the value is not correct\n", file, j+1);
					exit(0);
				}
				t[atoi(value)-1]++;
				valueP = 0;
				value[0] = '\0';
			}
			if(buf[0] != '\n')
			{
				while(buf[0] != '\n')
				{
					fread(buf,1,1,fp);
					if(buf[0] == ',' || buf[0] == '\n')
						j++;
				}
				printf("%s: (  V) %d values read while 16 was expected\n", file, j);
				exit(0);
			}

			if(t[0] != 4)
			{
				printf("%s: (  V) the value 1 in the V table did not occur 4 times\n", file);
				exit(0);
			}
			else if(t[1] != 12)
			{
				printf("%s: (  V) the value 2 in the V table did not occur 12 times\n", file);
				exit(0);
			}
		}
		else if(strcmp(header, "PC1") == 0)
		{
			PC1Flag++;
			if(PC1Flag != 1)
				printError();
			int t[65];
			int j = 0;
			char *value;
			value = (char *) malloc(2*sizeof(char*));
			int valueP = 0;

			for(j = 0; j < 65; j++)
				t[j] = 0;

			for(j = 0; j < 56; j++)
			{
				if((size = fread(buf,1,1,fp)) != 1)
				{
					printf("%s: (PC1) %d values read while 56 was expected\n", file, j);
					exit(0);
				}
				while(buf[0] != '\n' && buf[0] != ',')
				{
					value[valueP] = buf[0];
					if((size = fread(buf,1,1,fp)) != 1)
						printError();
					valueP++;
				}
				value[valueP] = '\0';
				if(!isDigit(value))
				{
					printf("%s: (  V) at %d, the value is not correct\n", file, j+1);
					exit(0);
				}
				t[atoi(value)]++;
				valueP = 0;
				value[0] = '\0';
			}
			if(buf[0] != '\n')
			{
				while(buf[0] != '\n')
				{
					fread(buf,1,1,fp);
					if(buf[0] == ',' || buf[0] == '\n')
						j++;
				}
				printf("%s: (PC1) %d values read while 56 was expected\n", file, j);
				exit(0);
			}

			for(j = 0; j < 65; j++)
			{
				if((j % 8) == 0)
				{
					if(t[j] != 0)
					{
						printf("%s: (PC1) the value %d occurs which should not occur\n", file ,j);
						exit(0);
					}
				}
				else
				{
					if(t[j] != 1)
					{
						printf("%s: (PC1) the value %d did not occur\n", file ,j);
						exit(0);
					}
				}
			}
		}
		else if(strcmp(header, "PC2") == 0)
		{
			PC2Flag++;
			if(PC2Flag != 1)
				printError();
			int t[57];
			int j = 0;
			char *value;
			value = (char *) malloc(2*sizeof(char*));
			int valueP = 0;

			for(j = 0; j < 57; j++)
				t[j] = 0;

			for(j = 0; j < 48; j++)
			{
				if((size = fread(buf,1,1,fp)) != 1)
				{
					printf("%s: (PC2) %d values read while 48 was expected\n", file, j);
					exit(0);
				}
				while(buf[0] != '\n' && buf[0] != ',')
				{
					value[valueP] = buf[0];
					if((size = fread(buf,1,1,fp)) != 1)
						printError();
					valueP++;
				}
				value[valueP] = '\0';
				if(!isDigit(value))
				{
					printf("%s: (  V) at %d, the value is not correct\n", file, j+1);
					exit(0);
				}
				t[atoi(value)]++;
				valueP = 0;
				value[0] = '\0';
			}
			if(buf[0] != '\n')
			{
				while(buf[0] != '\n')
				{
					fread(buf,1,1,fp);
					if(buf[0] == ',' || buf[0] == '\n')
						j++;
				}
				printf("%s: (PC2) %d values read while 48 was expected\n", file, j);
				exit(0);
			}

			for(j = 0; j < 57; j++)
			{
				if(t[j] > 1)
				{
					printf("%s: (PC2) the value %d occurs too many times\n", file, j);
					exit(0);
				}
			}
		}
		else
			printError();
		header[0] = '\0';
		headerPoint = 0;

	}
}
