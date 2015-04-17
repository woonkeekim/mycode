#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "keyexpand.h"
#include "tablecheck.h"
#include "encrypt.h"

void invShiftRows(unsigned char state[][4])
{
	int i = 0;
	unsigned char tmp = state[1][3];
	for(i = 3; i >= 1; i--)
	{
		state[1][i] = state[1][i-1];
	}
	state[1][0] = tmp;

	tmp = state[2][2];
	unsigned char tmp2 = state[2][3];
	for(i = 3; i >= 2; i--)
	{
		state[2][i] = state[2][i-2];
	}
	state[2][0] = tmp;
	state[2][1] = tmp2;

	tmp = state[3][0];
	for(i = 0; i < 3; i++)
	{
		state[3][i] = state[3][i+1];
	}
	state[3][3] = tmp;
	
}

void createInvSTable(char *buffer, unsigned char invSTable[][16])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 16; i++)
	{
		for(j = 0; j < 16; j++)
		{
			invSTable[hexVal(buffer[i*32 + j*2])][hexVal(buffer[i*32+j*2+1])] = i;
			invSTable[hexVal(buffer[i*32 + j*2])][hexVal(buffer[i*32+j*2+1])] <<= 4;
			invSTable[hexVal(buffer[i*32 + j*2])][hexVal(buffer[i*32+j*2+1])] += j;
		}
	}
}

void decryptFromKey(char *key, char *table)
{
	if(strlen(key) != 32)
	{
		fprintf(stderr, "Key format is invalid\n");
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

	FILE *fp = fopen(table, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "%s does not exist\n", table);
		exit(0);
	}

	char buf[1];
	char buffer[1000];
	memset(buffer, 0, 1000 * sizeof(char));
	ssize_t size;
	int j =0;
	unsigned char invSTable[16][16];
	memset(invSTable, 0, 16 * 16 * sizeof(invSTable[0][0]));
	unsigned char sTable[16][16];
	memset(sTable, 0, 16 * 16 * sizeof(invSTable[0][0]));
	unsigned char keys[4];
	memset(keys, 0, 4 * sizeof(keys[0]));

	int invpFlag = 0;
	int sFlag = 0;
	i = 0;
	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] == '\n')
		{
			if(strncmp(buffer, "S=", 2) == 0)
			{
				sFlag++;
				buffer[i] = '\0';
				if(!sCheck(buffer))
				{
					fprintf(stderr, "Table format is invalid\n");
					exit(0);
				}
				createSTable(buffer+2, sTable);
				createInvSTable(buffer+2, invSTable);
				if(invpFlag > 0)
					break;
				i = 0;
			}
			else if(strncmp(buffer, "P=", 2) == 0)
				i = 0;
			else if(strncmp(buffer, "INVP=", 5) == 0)
			{
				invpFlag++;
				buffer[i] = '\0';
				if(!pCheck(buffer, 5))
				{
					fprintf(stderr, "INVP format is invalid\n");
					exit(0);
				}
				createKey(buffer+5, keys);
				if(sFlag > 0)
					break;
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
	generateState(buffer, state);

	unsigned char rotated_key[4][4];
	memset(rotated_key, 0, 4 * 4 * sizeof(rotated_key[0][0]));

	unsigned char w[44][4];
	memset(w, 0, 4 * 44 * sizeof(w[0][0]));
	generateW(key, sTable, w);

	unsigned char round_key[4][4];
	memset(round_key, 0, 4 * 4 * sizeof(round_key[0][0]));
	generateRoundKey(w, round_key, 10);

	int roundIterator = 0;
	printf("round[%2d].iinput   ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	printf("round[%2d].ik_sch   ", roundIterator);
	for(i =0 ; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", round_key[j][i]);
		}
	}
	printf("\n");

	xorOperation(state, round_key);
	for(roundIterator = 9; roundIterator >= 1; roundIterator--)
	{
		printf("round[%2d].istart   ", 10-roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		printf("round[%2d].is_row   ", 10-roundIterator);
		invShiftRows(state);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		printf("round[%2d].is_box   ", 10-roundIterator);
		subBytes(invSTable, state);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
		printf("round[%2d].ik_sch   ", 10-roundIterator);
		generateRoundKey(w, round_key, roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", round_key[j][i]);
			}
		}
		printf("\n");

		xorOperation(state, round_key);
		printf("round[%2d].ik_add   ", 10-roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}	
		printf("\n");

		generateMixColTable(keys, rotated_key);
		MixCol(rotated_key,state);
	}
		
	printf("round[%2d].istart   ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	invShiftRows(state);
	printf("round[%2d].is_row   ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	subBytes(invSTable,state);	
	printf("round[%2d].is_box   ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	printf("round[%2d].ik_sch   ", 10-roundIterator);
	generateRoundKey(w, round_key, roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", round_key[j][i]);
		}
	}
	printf("\n");
	xorOperation(state, round_key);
	printf("round[%2d].ioutput  ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");
}
void decryptFromFile(char *key, char *table, char *input)
{
	if(strlen(key) != 32)
	{
		fprintf(stderr, "Key format is invalid\n");
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
	memset(buffer, 0, 1000 * sizeof(char));
	ssize_t size;
	int j =0;
	unsigned char invSTable[16][16];
	memset(invSTable, 0, 16 * 16 * sizeof(invSTable[0][0]));
	unsigned char sTable[16][16];
	memset(sTable, 0, 16 * 16 * sizeof(invSTable[0][0]));
	unsigned char keys[4];
	memset(keys, 0, 4 * sizeof(keys[0]));

	int invpFlag = 0;
	int sFlag = 0;
	i = 0;
	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] == '\n')
		{
			if(strncmp(buffer, "S=", 2) == 0)
			{
				sFlag++;
				buffer[i] = '\0';
				if(!sCheck(buffer))
				{
					fprintf(stderr, "Table format is invalid\n");
					exit(0);
				}
				createSTable(buffer+2, sTable);
				createInvSTable(buffer+2, invSTable);
				if(invpFlag > 0)
					break;
				i = 0;
			}
			else if(strncmp(buffer, "P=", 2) == 0)
			{	
					i = 0;
			}
			else if(strncmp(buffer, "INVP=", 5) == 0)
			{
				invpFlag++;
				buffer[i] = '\0';
				if(!pCheck(buffer, 5))
				{
					fprintf(stderr, "INVP format is invalid\n");
					exit(0);
				}
				createKey(buffer+5, keys);
				if(sFlag > 0)
					break;
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
	fclose(fp1);

	unsigned char state[4][4];
	memset(state, 0, 4 * 4 * sizeof(state[0][0]));
	generateState(buffer, state);

	unsigned char rotated_key[4][4];
	memset(rotated_key, 0, 4 * 4 * sizeof(rotated_key[0][0]));

	unsigned char w[44][4];
	memset(w, 0, 4 * 44 * sizeof(w[0][0]));
	generateW(key, sTable, w);

	unsigned char round_key[4][4];
	memset(round_key, 0, 4 * 4 * sizeof(round_key[0][0]));
	generateRoundKey(w, round_key, 10);

	int roundIterator = 0;
	printf("round[%2d].iinput   ", roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	printf("round[%2d].ik_sch   ", roundIterator);
	for(i =0 ; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", round_key[j][i]);
		}
	}
	printf("\n");

	xorOperation(state, round_key);
	for(roundIterator = 9; roundIterator >= 1; roundIterator--)
	{
		printf("round[%2d].istart   ", 10-roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		printf("round[%2d].is_row   ", 10-roundIterator);
		invShiftRows(state);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");

		printf("round[%2d].is_box   ", 10-roundIterator);
		subBytes(invSTable, state);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}
		printf("\n");
		printf("round[%2d].ik_sch   ", 10-roundIterator);
		generateRoundKey(w, round_key, roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", round_key[j][i]);
			}
		}
		printf("\n");

		xorOperation(state, round_key);
		printf("round[%2d].ik_add   ", 10-roundIterator);
		for(i = 0; i < 4; i++)
		{
			for(j = 0; j < 4; j++)
			{
				printf("%02x", state[j][i]);
			}
		}	
		printf("\n");

		generateMixColTable(keys, rotated_key);
		MixCol(rotated_key,state);
	}
		
	printf("round[%2d].istart   ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	invShiftRows(state);
	printf("round[%2d].is_row   ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	subBytes(invSTable,state);	
	printf("round[%2d].is_box   ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

	printf("round[%2d].ik_sch   ", 10-roundIterator);
	generateRoundKey(w, round_key, roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", round_key[j][i]);
		}
	}
	printf("\n");
	xorOperation(state, round_key);
	printf("round[%2d].ioutput  ", 10-roundIterator);
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 4; j++)
		{
			printf("%02x", state[j][i]);
		}
	}
	printf("\n");

}
