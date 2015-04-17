#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tablecheck.h"

void generateATable(unsigned char k[], unsigned char a[][4])
{
	int i = 0;
	int j = 0;
	a[0][0] = k[0];
	a[0][1] = k[3];
	a[0][2] = k[2];
	a[0][3] = k[1];
	for(i = 1; i < 4; i++)
	{
		unsigned char tmp = a[i-1][3];
		for(j = 1; j < 4; j++)
		{
			a[i][j]= a[i-1][j-1];
		}
		a[i][0] = tmp;
	}
}

int isOne(unsigned char rem[])
{
	int i = 0;
	for(i = 1; i < 5; i++)
	{
		if(rem[i] != 0)
			return 0;
	}
	if(rem[0] == 1)
		return 1;
	else return 0;
}
int isZero(unsigned char rem[])
{
	int i = 0;
	for(i = 0; i < 5; i++)
	{
		if(rem[i] != 0)
			return 0;
	}
	return 1;
}
int Exp(unsigned char a[])
{
	int i = 0;
	for(i = sizeof(a); i >= 0; i--)
	{
		if(a[i] != 0)
		{
			return i;
		}
	}
	return 0;
}

void divide(unsigned char rem[][5], unsigned char quote[])
{
	int i = 0; int j = 0; int k = 0;
	unsigned char remMul[4][8];
	memset(remMul, 0, 4 * 8 * sizeof(remMul[0][0]));
	for(i = 0; i  < 4; i++)
	{
		remMul[i][0] = rem[1][i];
	}
	computeMultiplication(remMul);

	unsigned char a[4][4];
	memset(a,0,4*4*sizeof(a[0][0]));
	generateATable(rem[1], a);
/*
	for(i = 0; i < 4; i++)
	{
		for(j = 0; j < 5; j++)
		{
			printf("%02x  ", a[i][j]);
		}
		printf("\n");
	}
*/
	int rem0Exp = Exp(rem[0]);
	int rem1Exp = Exp(rem[1]);
	for(i = rem0Exp - rem1Exp; i >= 0; i--)
	{
		for(j = 0; j < 256; j++)
		{
			unsigned char tmp = 0;
			for(k = 0; k < 8; k++)
			{
				if(((j >> k) & 1) == 1)
				{
					tmp ^= remMul[rem1Exp][k];
				}
			}
			if(rem0Exp == 0 && rem1Exp == 0)
			{
				if(tmp == (0x01 ^ rem[0][0]))
					break;
			}
			else
				if(tmp == rem[0][rem0Exp])
					break;
		}

		quote[i] = j;
		unsigned char tmp1[4];
		memset(tmp1, 0, 4 * sizeof(tmp1[0]));
		for(j = 0; j < 8; j++)
		{
			if(((quote[rem0Exp-rem1Exp] >> j) &1) == 1)
			{
				for(k = rem1Exp; k >= 0; k--)
				{
					rem[0][k+rem0Exp-rem1Exp] ^= remMul[k][j];
				}
			}
		}

		rem0Exp = Exp(rem[0]);
		rem1Exp = Exp(rem[1]);
	}

	for(j =0; j < 5; j++)
	{
		unsigned char tmp = rem[0][j];
		rem[0][j] = rem[1][j];
		rem[1][j] = tmp;
	}
}

void auxMultiplication(unsigned char aux[][4], unsigned char quote[])
{
	int i = 0; int j = 0; int k = 0;
	unsigned char auxMul[4][8];
	memset(auxMul, 0, 4 * 8 * sizeof(auxMul[0][0]));
	unsigned char a[4][4];
	memset(a, 0, 4 * 4 * sizeof(a[0][0]));
	generateATable(aux[1], a);

	for(i = 0; i < 4; i++)
	{
		memset(auxMul, 0, 4 * 8 * sizeof(auxMul[0][0]));
		for(j = 0; j < 4; j++)
			auxMul[j][0] = a[i][j];
		computeMultiplication(auxMul);
	
		for(j = 0; j < 4; j++)
		{
			for(k = 0; k < 8; k++)
			{
				if(((quote[j] >> k) & 1) == 1)
				{
					aux[2][i] ^= auxMul[j][k];
				}
			}
		}
	}
	for(i = 0; i < 4; i++)
	{
		aux[2][i] ^= aux[0][i];
	}
	for(j = 0; j < 2; j++)
	{
		for(i = 3; i>= 0; i--)
		{
			aux[j][i] = aux[j+1][i];
		}
	}
	for(i = 3; i >= 0; i--)
	{
		aux[2][i] = 0;
	}
/*
	for(j = 0; j < 3; j++)
	{
		for(i = 3; i>= 0; i--)
		{
			printf("%02x  ", aux[j][i]);
		}
		printf("\n");
	}
	printf("\n");
*/
}

void inverse(char *poly)
{
	if(strlen(poly) != 8)
	{
		fprintf(stderr, "Poly form is invalid\n");
		exit(0);
	}

	unsigned char aux[3][4];
	memset(aux, 0, 3 * 4 * sizeof(aux[0][0]));
	aux[1][0] = 1;

	int i =0; int j = 0;
	unsigned char quote[4];
	memset(quote, 0, 4 * sizeof(quote[0]));

	unsigned char rem[3][5];
	memset(rem, 0, 3* 5 * sizeof(rem[0][0]));

/*
	rem[0][0] = 0x01;
	rem[0][4] = 0x01;
	rem[1][0] = 0x02;
	rem[1][1] = 0x01;
	rem[1][2] = 0x01;
	rem[1][3] = 0x03;
	aux[1][0] = 0x01;
		memset(quote, 0, 4 * sizeof(quote[0]));
		divide(rem, quote);

		memset(quote, 0, 4 * sizeof(quote[0]));
		divide(rem, quote);
*/
	unsigned char first[4];
	memset(first, 0, 4*sizeof(first[0]));
	rem[0][0] = 0x01;
	rem[0][4] = 0x01;
	aux[1][0] = 0x01;

	j = 0;
	for(i = 6; i >= 0; i=i-2)
	{
		if(hexVal(poly[j]) == -1)
		{
			fprintf(stderr, "Invalid poly input\n");
			exit(0);
		}
		rem[1][i/2] = hexVal(poly[j]);
		first[i/2] = hexVal(poly[j]);
		rem[1][i/2] <<= 4;
		first[i/2] <<= 4;
		if(hexVal(poly[j+1]) == -1)
		{
			fprintf(stderr, "Invalid poly input\n");
			exit(0);
		}
		rem[1][i/2] += hexVal(poly[j+1]);
		first[i/2] += hexVal(poly[j+1]);
		j=j+2;
	}

	for(i = 1; i < 3; i++)
	{
		printf("i=%d, rem[i]=", i);
		for(j = 3; j >= 0; j--)
		{
			printf("{%02x}", rem[i-1][j]);
		}
		printf(", quo[i]=");
		for(j = 3; j>= 0; j--)
		{
			printf("{%02x}", quote[j]);
		}
		printf(", aux[i]=");
		for(j = 3; j >= 0; j--)
		{
			printf("{%02x}", aux[i-1][j]);
		}
		printf("\n");
	}

	for(i = 3; i < 10; i++)
	{
		memset(quote, 0, 4 * sizeof(quote[0]));
		divide(rem, quote);
		auxMultiplication(aux,quote);
		printf("i=%d, rem[i]=", i);
		for(j = 3; j >= 0; j--)
		{
			printf("{%02x}", rem[1][j]);
		}
		printf(", quo[i]=");
		for(j = 3; j>= 0; j--)
		{
			printf("{%02x}", quote[j]);
		}
		printf(", aux[i]=");
		for(j = 3; j >= 0; j--)
		{
			printf("{%02x}", aux[1][j]);
		}
		printf("\n");

		if(isZero(rem[1]))
		{
			for(j = 3; j >= 0; j--)
			{
				printf("{%02x}", first[j]);
			}
			printf(" does not have a multiplicative inverse.");
			exit(0);
		}
		else if(isOne(rem[1]))
		{
			printf("Multiplicative inverse of ");
			for(j = 3; j >= 0; j--)
			{
				printf("{%02x}", first[j]);
			}
			printf(" is ");
			for(j = 3; j >= 0; j--)
			{
				printf("{%02x}", aux[1][j]);
			}
			printf("\n");
			exit(0);
		}
	}
}
