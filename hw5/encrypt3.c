#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tablecheck.h"
#include "encrypt.h"
#include "decrypt.h"
void encrypt3FromKey(char *key, char *tablefile)
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
	int i = 0;

	
	char *key1;
	key1 = (char *) malloc (16*sizeof(char*));
	char *key2;
	key2 = (char *) malloc (16*sizeof(char*));
	char *key3;
	key3 = (char *) malloc (16*sizeof(char*));
	for(i = 0; i < 16; i++)
	{
		key1[i] = key[i];
	}
	key1[i] = '\0';
	for(i = 16; i < 32; i++)
	{
		key2[i-16] = key[i];
	}
	key2[16] = '\0';
	for(i = 32; i < 48; i++)
	{
		key3[i-32] = key[i];
	}
	key3[16] = '\0';


	unsigned char keys1[16][6];
	unsigned char keys2[16][6];
	unsigned char keys3[16][6];

	keyGenerator(key1, keys1, PC1, V, PC2);
	keyGenerator(key2, keys2, PC1, V, PC2);
	keyGenerator(key3, keys3, PC1, V, PC2);

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
		encryption(buf1, keys1, S, P, IP, invIP, E, result, done);
		decryption(result, keys2, S, P, IP, invIP, E, result, done);
		encryption(result, keys3, S, P, IP, invIP, E, result, done);
		int j =0;
		for(j = 0; j < 8; j++)
		{
			printf("%c", result[j]);
		}
		done++;
	} 
}
void encrypt3(char *key, char *tablefile, char *input)
{
	// get the key first
	FILE *tableFile = fopen(tablefile, "r");
	if(tableFile == NULL)
	{
		printf("the file %s does not exist\n", tablefile);
	}
	FILE *inputFile = fopen(input, "r");
	if(inputFile == NULL)
	{
		printf("The file %s does not exist", input);
		exit(0);
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
	int i = 0;

	
	char *key1;
	key1 = (char *) malloc (16*sizeof(char*));
	char *key2;
	key2 = (char *) malloc (16*sizeof(char*));
	char *key3;
	key3 = (char *) malloc (16*sizeof(char*));
	for(i = 0; i < 16; i++)
	{
		key1[i] = key[i];
	}
	key1[i] = '\0';
	for(i = 16; i < 32; i++)
	{
		key2[i-16] = key[i];
	}
	key2[16] = '\0';
	for(i = 32; i < 48; i++)
	{
		key3[i-32] = key[i];
	}
	key3[16] = '\0';


	unsigned char keys1[16][6];
	unsigned char keys2[16][6];
	unsigned char keys3[16][6];

	keyGenerator(key1, keys1, PC1, V, PC2);
	keyGenerator(key2, keys2, PC1, V, PC2);
	keyGenerator(key3, keys3, PC1, V, PC2);

	unsigned char buf1[8];
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
		encryption(buf1, keys1, S, P, IP, invIP, E, result, done);
		decryption(result, keys2, S, P, IP, invIP, E, result, done);
		encryption(result, keys3, S, P, IP, invIP, E, result, done);
		int j =0;
		for(j = 0; j < 8; j++)
		{
			printf("%c", result[j]);
		}
		done++;
	} 
	fclose(inputFile);
}
