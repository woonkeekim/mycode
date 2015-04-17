#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tablecheck.h"
void generateRCON(unsigned char r[][4])
{
	int i = 1;
	for(i = 2; i < 11; i++)
	{
		r[i][0] = r[i-1][0] * 2;
		if((int)(r[i-1][0] * 2) > 255)
		{
			r[i][0] = (unsigned char)(r[i][0] ^ 0x1b);
		} 
	}
}
void copyToTmp(unsigned char *tmp, unsigned char *w)
{
	int i =0;
	for(i = 0;i < 4; i++)
	{
		tmp[i] = w[i];
	}

}
void firstFour(unsigned char w[44][4], char *key)
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			w[i][j] = hexVal(key[i*8+j*2]);
			w[i][j] <<= 4;
			w[i][j] += hexVal(key[i*8+j*2+1]);
		}
	}
}
void subword(unsigned char sTable[][16], unsigned char *dest)
{
	int i = 0;
	for(i = 0; i < 4; i++)
	{
		int j = dest[i] >> 4;
		j &= 15;
		int k = dest[i] & 15;
		dest[i] = sTable[j][k];
	}
}
void rotate(unsigned char *input)
{
	unsigned char tmp = input[0];
	int i = 0;
	for(i = 0; i < sizeof(input)-1; i++)
	{
		input[i] = input[i+1];
	}
	input[sizeof(input)-1] = tmp;
}
void createSTable(char *buffer, unsigned char sTable[][16])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
			sTable[i][j] = hexVal(buffer[i*32+2*j]);
			sTable[i][j] <<= 4;
			sTable[i][j] += hexVal(buffer[i*32+2*j+1]);
		}
	}
}

void keyexpand(char *key, char* file)
{
	if(strlen(key) != 32)
	{
		fprintf(stderr, "Key is invalid\n");
		exit(0);
	}
	int i =0;
	for(i = 0; i < 32; i++)
	{
		if(hexVal(key[i]) == -1)
		{
			fprintf(stderr, "Key is invalid\n");
			exit(0);
		}
	}	
	i = 0;
	FILE *fp;
	fp = fopen(file, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "%s does not exist\n", file);
		exit(0);
	}
	char buf[1];
	char buffer[1000];
	memset(buffer, 0, 1000*sizeof(char));
	ssize_t size;
	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] == '\n')
		{
			if(strncmp(buffer, "S=", 2) == 0)
			{
				buffer[i] = '\0';
				if(!sCheck(buffer))
				{
					fprintf(stderr, "Table format is invalid\n");
					exit(0);
				}
				break;
			}
			else
			{
				i = 0;
			}
		}
		else
		{
			buffer[i++] = buf[0];
		}
	}
	unsigned char sTable[16][16];
	memset(sTable, 0, 16 * 16 * sizeof(sTable[0][0]));
	createSTable(buffer+2, sTable);
	int j = 0;
	unsigned char RCON[11][4];
	memset(RCON, 0, 11 * 4 * sizeof(RCON[0][0]));

	RCON[1][0] = 0x01;
	generateRCON(RCON);

	unsigned char w[44][4];
	memset(w, 0, 4*44* sizeof(w[0][0]));
	// generate first four key
	firstFour(w, key);

	for(i = 4; i < 44; i++)
	{
		unsigned char tmp[4];
		memset(tmp, 0, 4 * sizeof(tmp[0]));
		copyToTmp(tmp, w[i-1]);
		if((i % 4) == 0)
		{
			rotate(tmp);
			subword(sTable, tmp);
			for(j = 0; j < 4; j++)
			{
				tmp[j] = (unsigned char)(tmp[j] ^ RCON[i/4][j]);
			}
		}
		for(j = 0; j < 4; j++)
		{
			w[i][j] = (unsigned char)(w[i-4][j] ^ tmp[j]);
		}
	}
	for(i = 0; i < 44; i++)
	{
		printf("w[%2d]: ", i);
		for(j = 0; j < 4; j++)
		{
			printf("%02x", w[i][j]);
		}
		printf("\n");
	}
}
