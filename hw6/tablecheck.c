#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>

#define AESPOLY 0x11B
int isInverse(unsigned char rotated_a[][4], unsigned char a[][8], unsigned char b[])
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
	if(d[0] == 1 && d[1] == 0 && d[2] == 0 && d[3] == 0)
		return 1;
	else
		return 0;
}

void createRotatedTable(unsigned char a[][8], unsigned char r[][4])
{
	r[0][0]= 0;
	r[0][1]= 3;
	r[0][2]= 2;
	r[0][3]= 1;
	r[1][0]= 1;
	r[1][1]= 0;
	r[1][2]= 3;
	r[1][3]= 2;
	r[2][0]= 2;
	r[2][1]= 1;
	r[2][2]= 0;
	r[2][3]= 3;
	r[3][0]= 3;
	r[3][1]= 2;
	r[3][2]= 1;
	r[3][3]= 0;

}

void computeMultiplication(unsigned char a[][8])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 4; i++)
	{
		for(j = 1; j < 8; j++)
		{
			a[i][j] = a[i][j-1] * 2;
			if(a[i][j-1] * 2 > 255)
			{
				a[i][j] ^= AESPOLY;
			}
		}
	}
}

int hexVal(char a)
{
	if(a >= '0' && a <= '9')
		return (a-'0');
	else if(a >= 'a' && a <= 'f')
		return (a-'a')+10;
	else
		return -1;
}

int sCheck(char *input)
{
	int i = 0;
	char buffer[256] = {0,};
	for(i = 2; i < strlen(input) - 1; i=i+2)
	{
		if((input[i] < '0' || input[i] > '9') && (input[i] < 'a' || input[i] > 'f'))
			return 0;
		else if((input[i+1] < '0' || input[i+1] > '9') && (input[i+1] < 'a' || input[i+1] > 'f'))
			return 0;
		else
		{
			int p = 0;
			if(input[i] >= '0' && input[i] <= '9')
				p = input[i] - '0';
			else
				p = input[i] - 'a' + 10;
			p <<= 4;
			if(input[i+1] >= '0' && input[i+1] <= '9')
				p += input[i+1] - '0';
			else
				p += input[i+1] - 'a' + 10;
			buffer[p]++;
		}
	}
	for(i = 0; i < 256; i++)
	{
		if(buffer[i] != 1)
		{
			return 0;
		}
	}
	return 1;
}
int pCheck(char *input, int start)
{
	int i = 0;
	for(i = start; i < strlen(input) - 1; i=i+2)
	{
		if((input[i] < '0' || input[i] > '9') && (input[i] < 'a' || input[i] > 'f'))
			return 0;
		else if((input[i+1] < '0' || input[i+1] > '9') && (input[i+1] < 'a' || input[i+1] > 'f'))
			return 0;
	}
	return 1;
}

int tablecheck(char *tablefile)
{
	FILE *fp = fopen(tablefile, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "%s does not exist\n", tablefile);
		exit(0);
	}
	char buf[1];
	char buffer[1000] = {0,};
	ssize_t size;
	int i = 0;
	int sHeader, pHeader,invpHeader= 0;
	unsigned char a[4][8];
	memset(a, 0, sizeof(a[0][0]) * 4 * 8);
	unsigned char b[4];
	memset(b, 0, sizeof(unsigned char)*4);
	while((size = fread(buf, 1, 1, fp)) != 0)
	{
		if(buf[0] == '\n')
		{
			if(strncmp(buffer, "S=", 2) == 0)
			{
				if(!sCheck(buffer))
				{
					fprintf(stderr, "S in the table format is wrong\n");
					return 0;
				}
				i = 0;
				sHeader++;
			}

			else if(strncmp(buffer, "P=", 2) == 0)
			{
				if(!pCheck(buffer,2))
				{
					fprintf(stderr, "P in the table format is wrong\n");
					return 0;
				}
				if(strlen(buffer+2) != 8)
				{
					fprintf(stderr, "P in table is invalid\n");
					return 0;
				}
				//create a values;;
				int j = 0;
				int k = 0;
				for(j = strlen(buffer) - 2; j >= 0; j=j-2)
				{
					a[k][0] += hexVal(buffer[j]);
					a[k][0] <<= 4;
					a[k][0] += hexVal(buffer[j+1]);
					k++;
				}
				i = 0;
				pHeader++;

			}
			else if(strncmp(buffer, "INVP=", 5) == 0)
			{
				if(!pCheck(buffer, 5))
				{
					fprintf(stderr, "INVP in the table format is wrong\n");
					return 0;
				}
				if(strlen(buffer+5) != 8)
				{
					fprintf(stderr, "INVP in table is invalid\n");
					return 0;
				}
				//create b values;
				int j = 0;
				int k = 0;
				for(j = strlen(buffer) - 2; j >= 5; j=j-2)
				{
					b[k] += hexVal(buffer[j]);
					b[k] <<= 4;
					b[k] += hexVal(buffer[j+1]);
					k++;
				}
				i = 0;
				invpHeader++;
			}
			else
			{
				fprintf(stderr, "Table contains invalid header\n");
				return 0;
			}
			memset(buffer, 0, 1000);

		}
		else
		{
			buffer[i] = buf[0];
			i++;
		}
	}
	if(invpHeader != 1 || pHeader != 1 || sHeader != 1)
	{
		fprintf(stderr, "Table format is wrong\n");
		return 0;
	}
	computeMultiplication(a);
	unsigned char rotated_a[4][4];
	memset(rotated_a, 0, sizeof(rotated_a[0][0]) * 4 * 4);
	createRotatedTable(a, rotated_a);	
	if(!isInverse(rotated_a, a, b))
	{
		fprintf(stderr, "Inverse match failed\n");
		return 0;
	}

	return 1;
}

