#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablecheck.h"
#include "encrypt.h"
void decryption(unsigned char *buf1, unsigned char keys[][6], int S[][64], int *P, int *IP, int *invIP, int *E, unsigned char *result, int done)
{
	int i = 0;
	unsigned char* message;
	message = (unsigned char*) malloc(8 * sizeof(unsigned char*));
	for(i = 0; i < 64; i++)
	{
		message[i/8] <<= 1;
		message[i/8] += (buf1[(IP[i]-1)/8] >> (7-((IP[i]-1)%8)))&1;
	}
	for(i = 16; i > 0; i--)
	{
		int j = 0;
		if(done == 0)
		{
			fprintf(stderr,"(L%d,R%d)=", (16-i)%17, (16-i)%17);
			for(j = 0; j < 8; j++)
			{
				fprintf(stderr, "%02x", message[j]);
			}
			fprintf(stderr, "\n");
		}
		unsigned char* Eout;
		Eout = (unsigned char*) malloc(6*sizeof(unsigned char*));
		for(j = 0; j < 48; j++)
		{
			if((j %8) == 0)
				Eout[j/8] = 0;
			Eout[j/8] <<= 1;
			Eout[j/8] += (message[4+(E[j]-1)/8] >> (7-((E[j]-1)%8)))&1;
		}

		unsigned char *Tout;
		Tout = (unsigned char*) malloc(8* sizeof(unsigned char*));
		for(j = 0; j < 48; j++)
		{
			if(j % 6 == 0)
				Tout[j/6] = 0;
			int bit = 0;
			bit = (Eout[j/8] >> (7-(j%8)))&1;
			int bit2 = 0;
			bit2 = (keys[i-1][j/8] >> (7-(j%8)))&1;
			bit ^= bit2;
			Tout[j/6] <<=1;
			Tout[j/6] += bit;
		}
		unsigned char *Tout2;
		Tout2 = (unsigned char*) malloc( 8 * sizeof(unsigned char*));
		for(j = 0; j < 8; j++)
		{
			int r = 0;
			r = 2*((Tout[j] >> 5)&1);
			r += Tout[j] &1;
			int c = 0;
			int k = 0;
			for(k = 0; k < 4; k++)
			{
				c <<= 1;
				c += (Tout[j] >> (4-k))&1;
			}
			Tout2[j] = S[j][r*16+c];
		}
		unsigned char *Tout3;
		Tout3 = (unsigned char*) malloc(4*sizeof(unsigned char*));
		for(j = 0; j < 32; j++)
		{
			Tout3[j/8] <<=1;
			Tout3[j/8] += (Tout2[(P[j]-1)/4] >> (3-((P[j]-1)%4)))&1; 
		}
		unsigned char* new_message;
		new_message = (unsigned char*) malloc(8 * sizeof(unsigned char*));
		int z = 0;
		for(z = 0; z < 4; z++)
		{
			new_message[z] = message[z+4];
		}
		//last part
		for(z = 0; z < 4; z++)
		{
			new_message[z+4] = message[z] ^ Tout3[z];
		}
		message = new_message;
	}
	int j = 0;
	if(done == 0)
	{
		fprintf(stderr,"(L%d,R%d)=", (16-i)%17, (16-i)%17);
		for(j = 0; j < 8; j++)
		{
			fprintf(stderr, "%02x", message[j]);
		}
		fprintf(stderr, "\n");
	}
	//swap left and right
	unsigned char* right;
	right = (unsigned char*) malloc(4*sizeof(unsigned char*));
	unsigned char* left;
	left = (unsigned char*) malloc(4*sizeof(unsigned char*));
	right = message+4;
	left = message;
	unsigned char* result_tmp;
	result_tmp = (unsigned char *) malloc(8*sizeof(unsigned char*));
	for(j = 0; j < 4; j++)
	{
		result_tmp[j] = right[j];
	}
	for(j = 0; j < 4; j++)
	{
		result_tmp[j+4] = left[j];
	}
	//result = (unsigned char *) malloc(8*sizeof(unsigned char*));
	for(j = 0; j < 64; j++)
	{
		result[j/8] <<= 1;
		result[j/8] += ((result_tmp[(invIP[j]-1)/8]) >> (7-((invIP[j]-1)%8)))&1;
	}

}
void decryptFromKey(char *key, char* tablefile)
{
	// get the key first
	FILE *tableFile = fopen(tablefile, "r");
	if(tableFile == NULL)
	{
		printf("the file %s does not exist\n", tablefile);
	}
	//parse the table file
	int IP[64];
	int E[48];
	int V[16];
	int P[32];
	int PC1[56];
	int PC2[48];
	int S[8][64];
	int invIP[64];
	tablecheck(tablefile);
	createTables(tableFile, IP, E, V, P, PC1, PC2, S);
	fclose(tableFile);
	inverseIP(IP, invIP);

	unsigned char keys[16][6];
	keyGenerator(key, keys, PC1, V, PC2);
	int i = 0;
	unsigned char buf1[8];
	int done = 0;
	int size =0;
	while((size = fread(buf1,1,8,stdin)) > 0)
	{
		if(size != 8)
		{
			for(i = size; i < 8; i++)
				buf1[i] = '\0';
		}
		unsigned char result[8]={0,};
		decryption(buf1, keys, S, P, IP, invIP, E, result, done);
		int j =0;
		for(j = 0; j < 8; j++)
		{
			printf("%c", result[j]);
		}

		done++;
	} 
}
void decrypt(char *key, char *tablefile, char *input)
{
	// get the key first
	FILE *tableFile = fopen(tablefile, "r");
	if(tableFile == NULL)
	{
		printf("the file %s does not exist\n", tablefile);
	}
	//parse the table file
	int IP[64];
	int E[48];
	int V[16];
	int P[32];
	int PC1[56];
	int PC2[48];
	int S[8][64];
	int invIP[64];
	tablecheck(tablefile);
	createTables(tableFile, IP, E, V, P, PC1, PC2, S);
	fclose(tableFile);
	inverseIP(IP, invIP);
	unsigned char keys[16][6];
	keyGenerator(key, keys, PC1, V, PC2);
	int i = 0;
	unsigned char buf1[8];
	FILE *inputFile = fopen(input, "r");
	if(inputFile == NULL)
	{
		printf("The file %s does not exist", input);
		exit(0);
	}
	int done = 0;
	int size =0;
	while((size = fread(buf1,1,8,inputFile)) > 0)
	{
		if(size != 8)
		{
			for(i = size; i < 8; i++)
				buf1[i] = '\0';
		}
		unsigned char result[8]={0,};
		decryption(buf1, keys, S, P, IP, invIP, E, result, done);
		int j =0;
		for(j = 0; j < 8; j++)
		{
			printf("%c", result[j]);
		}

		done++;
	} 
}

