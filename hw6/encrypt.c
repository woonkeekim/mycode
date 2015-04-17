#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keyexpand.h"
#include "tablecheck.h"

void xorOperation(unsigned char dest[][4], unsigned char src[][4])
{
	int i =0;
	int j =0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			dest[i][j] ^= src[i][j];
		}
	}
}

void MixCol(unsigned char rotated_key[][4], unsigned char state[][4])
{
	int i = 0; int j = 0; int k = 0; int l = 0;

	for(i = 0; i < 4; i++)
	{
		unsigned char tmpState[4];
		memset(tmpState, 0, 4 * sizeof(tmpState[0]));
		for(k = 0; k < 4; k++)
		{
			tmpState[k] = state[k][i];
		}

		for(j = 0; j < 4; j++)
		{
			unsigned char tmpMul[4][8];
			memset(tmpMul, 0, 4 * 8 * sizeof(tmpMul[0][0]));
			for(k = 0; k < 4; k++)
			{
				tmpMul[k][0] = rotated_key[j][k];
			}
			computeMultiplication(tmpMul);

			unsigned char d = 0;
			//memset(d 0, 4 * sizeof(d[0]));

			for(k = 0; k < 4; k++)
			{
				for(l = 0; l < 8; l++)
				{
					if(((tmpState[k] >> l) & 1) == 1)
					{
						d ^= tmpMul[k][l];
					}
				}
			}
			state[j][i] = d;
		}	
	}
}

void generateMixColTable(unsigned char keys[], unsigned char rotated_key[][4])
{
	int i = 0; int j = 0;
	rotated_key[0][0] = keys[3];
	rotated_key[0][1] = keys[0];
	rotated_key[0][2] = keys[1];
	rotated_key[0][3] = keys[2];
	for(i = 1; i < 4; i++)
	{
		unsigned char tmp;
		tmp = rotated_key[i-1][3];
		for(j = 1; j < 4; j++)
		{
			rotated_key[i][j] = rotated_key[i-1][j-1];
		}
		rotated_key[i][0] = tmp;
	}
}

void shiftRows(unsigned char state[][4])
{
	unsigned char tmp;
	tmp = state[1][0];
	int i = 0;
	for(i = 0; i < 3; i++)
	{
		state[1][i] = state[1][i+1];
	}
	state[1][3] = tmp;
	
	unsigned char tmp1;
	tmp = state[2][0];
	tmp1 = state[2][1];
	for(i = 0; i < 2; i++)
	{
		state[2][i] = state[2][i+2];
	}
	state[2][2] = tmp;
	state[2][3] = tmp1;

	unsigned char tmp2;
	tmp = state[3][0];
	tmp1 = state[3][1];
	tmp2 = state[3][2];
	state[3][0] = state[3][3];
	state[3][1] = tmp; 
	state[3][2] = tmp1; 
	state[3][3] = tmp2; 
}
void subBytes(unsigned char sTable[][16], unsigned char dest[][4])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			int k = dest[i][j] >> 4;
			k &= 15;
			int l = dest[i][j] & 15;
			dest[i][j] = sTable[k][l];			
		}
	}
}

void generateRoundKey(unsigned char w[][4], unsigned char roundKey[][4], int round)
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			roundKey[j][i] = w[4*round+i][j];
		}
	}
}

void generateW(char *key, unsigned char sTable[][16],  unsigned char w[][4])
{
	int i = 0;
	int j = 0;
	unsigned char RCON[11][4];
	memset(RCON, 0, 11 * 4 * sizeof(RCON[0][0]));

	RCON[1][0] = 0x01;
	generateRCON(RCON);
	
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
				tmp[j] = (unsigned char) (tmp[j] ^ RCON [i/4][j]);
			}
		}
		for(j = 0; j < 4; j++)
		{
			w[i][j] = (unsigned char) (w[i-4][j] ^ tmp[j]);
		}
	}
}
void createKey(char buffer[], unsigned char keys[])
{
	int i =0;
	for(i = 0; i < strlen(buffer) - 1; i=i+2)
	{
		keys[i/2] = hexVal(buffer[i]);
		keys[i/2] <<= 4;
		keys[i/2] += hexVal(buffer[i+1]);
	}
}
void generateState(char *buf, unsigned char state[][4])
{
	int i = 0;
	int j = 0;
	int bufCounter = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			state[j][i] = buf[bufCounter++];			
		}
	}
}

void encryptFromKey(char *key, char* table)
{
	if(strlen(key) != 32)
	{
		fprintf(stderr,"Key format is invalid\n");
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

	FILE *fp = fopen(table, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "%s does not exist\n", table);
		exit(0);
	}

	char buf[1];
	char buffer[1000];
	memset(buffer, 0, 1000*sizeof(char));
	ssize_t size;
	int j = 0;
	unsigned char sTable[16][16];
	memset(sTable, 0, 16 * 16 * sizeof(sTable[0][0]));
	unsigned char keys[4];
	memset(keys, 0, 4 * sizeof(keys[0]));

	int pFlag = 0;
	int sFlag = 0;

	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] == '\n')
		{
			if(strncmp(buffer, "S=", 2) ==0)
			{
				sFlag++;
				buffer[i] = '\0';
				if(!sCheck(buffer))
				{
					fprintf(stderr, "Table format is invalid\n");
					exit(0);
				}
				createSTable(buffer+2, sTable);
				if(pFlag > 0)
					break;
				i = 0;
			}
			else if(strncmp(buffer, "P=", 2) == 0)
			{
				pFlag++;
				buffer[i] ='\0';
				if(!pCheck(buffer,2))
				{
					fprintf(stderr, "P format is invalid\n");
					exit(0);
				}
				createKey(buffer+2, keys);
				if(sFlag > 0)
					break;
				i = 0;
			}
			else if(strncmp(buffer, "INVP=", 5) == 0)
			{
				i = 0;
			}
			else
			{
				fprintf(stderr, "Table file is invalid\n");
				exit(0);
			}
		}
		else
		{
			buffer[i++] = buf[0];
		}
	}
	fclose(fp);

	memset(buffer, 0, 1000 * sizeof(buffer[0]));

	i = 0;
	while((size = fread(buf, 1, 1, stdin)) > 0)
	{
		buffer[i] = buf[0];
		i++;
	}
	if(i != 16)
	{
		fprintf(stderr, "Input is not 16 bytes\n");
		exit(0);
	}
	unsigned char state[4][4];
	memset(state, 0, 4 * 4 * sizeof(state[0][0]));

	unsigned char rotated_key[4][4];
	memset(rotated_key, 0, sizeof(rotated_key[0][0]));

	generateState(buffer, state);

	unsigned char w[44][4];
	memset(w, 0, 4* 44 * sizeof(w[0][0]));
	generateW(key, sTable, w);
	
	unsigned char roundKey[4][4];
	memset(roundKey, 0, 4 * 4 * sizeof(roundKey[0][0]));
	generateRoundKey(w, roundKey, 0);
	int roundIterator = 0;

	printf("round[%2d].input    ", roundIterator);

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	printf("round[%2d].k_sch    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", roundKey[j][i]);
		}
	}
	printf("\n");

	xorOperation(state, roundKey);
	for(roundIterator = 1; roundIterator < 10; roundIterator++)
	{
		printf("round[%2d].start    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		subBytes(sTable,state);
		printf("round[%2d].s_box    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		shiftRows(state);
		printf("round[%2d].s_row    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		generateMixColTable(keys, rotated_key);
		MixCol(rotated_key, state);
		printf("round[%2d].m_col    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");	

		memset(roundKey, 0, 4 * 4 * sizeof(roundKey[0][0]));
		generateRoundKey(w, roundKey, roundIterator);

		printf("round[%2d].k_sch    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", roundKey[j][i]);
			}
		}
		printf("\n");
		xorOperation(state, roundKey);
	}
	printf("round[%2d].start    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
	subBytes(sTable,state);
	printf("round[%2d].s_box    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	shiftRows(state);
	printf("round[%2d].s_row    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
	memset(roundKey, 0, 4 * 4 * sizeof(roundKey[0][0]));
	generateRoundKey(w, roundKey, roundIterator);

	printf("round[%2d].k_sch    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", roundKey[j][i]);
		}
	}
	printf("\n");
	xorOperation(state, roundKey);
	printf("round[%2d].output   ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
}

void encryptFromFile(char *key, char *table, char *input)
{
	if(strlen(key) != 32)
	{
		fprintf(stderr,"Key format is invalid\n");
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
	FILE *fp = fopen(table, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "%s does not exist\n", table);
		exit(0);
	}

	FILE *fp1 = fopen(input, "r");
	if(fp1 == NULL)
	{
		fprintf(stderr, "%s does not exist\n", input);
		exit(0);
	}

	char buf[1];
	char buffer[1000];
	memset(buffer, 0, 1000*sizeof(char));
	ssize_t size;
	int j = 0;
	unsigned char sTable[16][16];
	memset(sTable, 0, 16 * 16 * sizeof(sTable[0][0]));
	unsigned char keys[4];
	memset(keys, 0, 4 * sizeof(keys[0]));

	int pFlag = 0;
	int sFlag = 0;

	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] == '\n')
		{
			if(strncmp(buffer, "S=", 2) ==0)
			{
				sFlag++;
				buffer[i] = '\0';
				if(!sCheck(buffer))
				{
					fprintf(stderr, "Table format is invalid\n");
					exit(0);
				}
				createSTable(buffer+2, sTable);
				if(pFlag > 0)
					break;
				i = 0;
			}
			else if(strncmp(buffer, "P=", 2) == 0)
			{
				pFlag++;
				buffer[i] ='\0';
				if(!pCheck(buffer,2))
				{
					fprintf(stderr, "P format is invalid\n");
					exit(0);
				}
				createKey(buffer+2, keys);
				if(sFlag > 0)
					break;
				i = 0;
			}
			else if(strncmp(buffer, "INVP=", 5) == 0)
			{
				i = 0;
			}
			else
			{
				fprintf(stderr, "Table file is invalid\n");
				exit(0);
			}
		}
		else
		{
			buffer[i++] = buf[0];
		}
	}
	fclose(fp);
	if(pFlag != 1 || sFlag != 1)
	{
		fprintf(stderr, "Table format is invalid\n");
		exit(0);
	}

	memset(buffer, 1000, 1000 * sizeof(buffer[0]));

	i = 0;
	while((size = fread(buf, 1, 1, fp1)) > 0)
	{
		buffer[i] = buf[0];
		i++;
	}
	if(i != 16)
	{
		fprintf(stderr, "Input is not 16 bytes\n");
		exit(0);
	}
	unsigned char state[4][4];
	memset(state, 0, 4 * 4 * sizeof(state[0][0]));

	unsigned char rotated_key[4][4];
	memset(rotated_key, 0, sizeof(rotated_key[0][0]));

	generateState(buffer, state);

	unsigned char w[44][4];
	memset(w, 0, 4* 44 * sizeof(w[0][0]));
	generateW(key, sTable, w);
	
	unsigned char roundKey[4][4];
	memset(roundKey, 0, 4 * 4 * sizeof(roundKey[0][0]));
	generateRoundKey(w, roundKey, 0);
	int roundIterator = 0;

	printf("round[%2d].input    ", roundIterator);

	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	printf("round[%2d].k_sch    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", roundKey[j][i]);
		}
	}
	printf("\n");

	xorOperation(state, roundKey);
	for(roundIterator = 1; roundIterator < 10; roundIterator++)
	{
		printf("round[%2d].start    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		subBytes(sTable,state);
		printf("round[%2d].s_box    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		shiftRows(state);
		printf("round[%2d].s_row    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		generateMixColTable(keys, rotated_key);
		MixCol(rotated_key, state);
		printf("round[%2d].m_col    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");	

		memset(roundKey, 0, 4 * 4 * sizeof(roundKey[0][0]));
		generateRoundKey(w, roundKey, roundIterator);

		printf("round[%2d].k_sch    ", roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", roundKey[j][i]);
			}
		}
		printf("\n");
		xorOperation(state, roundKey);
	}
	printf("round[%2d].start    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
	subBytes(sTable,state);
	printf("round[%2d].s_box    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	shiftRows(state);
	printf("round[%2d].s_row    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
	memset(roundKey, 0, 4 * 4 * sizeof(roundKey[0][0]));
	generateRoundKey(w, roundKey, roundIterator);

	printf("round[%2d].k_sch    ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", roundKey[j][i]);
		}
	}
	printf("\n");
	xorOperation(state, roundKey);
	printf("round[%2d].output   ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
}

