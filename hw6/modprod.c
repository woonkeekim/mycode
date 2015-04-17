#include <stdlib.h> 
#include <stdio.h> 
#include <string.h> 
#include "tablecheck.h"

#define AESPOLY 0x11B
void circleX(unsigned char rotated_a[][4], unsigned char a[][8], unsigned char b[], unsigned char d[])
{
        int i = 0;
        int j = 0;
        int k = 0;
        for(i = 0; i < 4; i++)
        {
                for(j = 0; j < 4; j++)
                {
                        for(k = 0; k < 8; k++)
                        {
                                if(((b[j] >> k) & 1) == 1)
                                        d[i] ^= a[rotated_a[i][j]][k];
                        }
                }
        }
	//{f1}{83}{f2}{a4} CIRCLEX {0d}{5f}{e0}{7d} = {03}{90}{74}{7e}
}

/*
void circleX(unsigned char rotated_a[][4], unsigned char a[][8], unsigned char b[])
{
        int i = 0;
        int j = 0;
        int k = 0;
        unsigned char d[4];
        memset(d, 0, 4*sizeof(unsigned char));
        for(i = 0; i < 4; i++)
        {
                for(j = 0; j < 4; j++)
                {
                        for(k = 0; k < 8; k++)
                        {
                                if(((b[j] >> k) & 1) == 1)
                                        d[i] ^= a[rotated_a[i][j]][k];
                        }
                }
        }
	//{f1}{83}{f2}{a4} CIRCLEX {0d}{5f}{e0}{7d} = {03}{90}{74}{7e}
	for(i = 3; i >= 0; i--)
	{
		printf("{%02x}", a[i][0]);
	}
	printf(" CIRCLEX ");
	for(i = 3; i >= 0; i--)
	{
		printf("{%02x}", b[i]);
	}
	printf(" = ");
	for(i = 3; i >= 0; i--)
	{
		printf("{%02x}", d[i]);
	}
	printf("\n");
}
*/

void modprod(char *p1, char *p2)
{ 
	int i = 0;
	unsigned char a[4][8];
	memset(a, 0, 4 * 8 * sizeof(a[0][0]));
	unsigned char b[4];
	memset(b, 0, 4* sizeof(b[0]));
	if(!pCheck(p1, 0))
	{
		fprintf(stderr, "Formatof p1 is wrong\n");
		exit(0);
	}
	if(!pCheck(p2, 0))
	{
		fprintf(stderr, "Formatof p2 is wrong\n");
		exit(0);
	}
	int k = 0;
	for(i = 6; i >= 0; i=i-2)
	{
		a[k][0] = hexVal(p1[i]);
		a[k][0] <<= 4;
		a[k][0] += hexVal(p1[i+1]);
		k++;
	}
	k = 0;
	for(i = 6; i >= 0; i=i-2)
	{
		b[k] = hexVal(p2[i]);
		b[k] <<= 4;
		b[k] += hexVal(p2[i+1]);
		k++;
	}
	unsigned char rotated_a[4][4];
	memset(rotated_a, 0, sizeof(rotated_a[0][0]) * 4 * 4);
	computeMultiplication(a);
	createRotatedTable(a, rotated_a);

	unsigned char d[4];
	memset(d, 0, sizeof(d[0]) * 4);

	circleX(rotated_a, a, b, d);
	for(i = 3; i >= 0; i--)
	{
		printf("{%02x}", a[i][0]);
	}
	printf(" CIRCLEX ");
	for(i = 3; i >= 0; i--)
	{
		printf("{%02x}", b[i]);
	}
	printf(" = ");
	for(i = 3; i >= 0; i--)
	{
		printf("{%02x}", d[i]);
	}
	printf("\n");

}
