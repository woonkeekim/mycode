#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "encrypt.h"
void decrypt(char *input)
{
	FILE *fp;
	fp = fopen(input, "r");
	if(fp == NULL)
	{
		printf("%s cannot be opened\n", input);
		exit(0);
	}

	char *buf;
	buf = (char *) malloc(2 * sizeof(char *));
	int size = 0;
	char *header;
	header = (char *) malloc(3 * sizeof(char *));
	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] != '\n')
		{
			buf[1] = '\0';
			strcat(header, buf); 
		}
		else
			break;
	}
	if(strcmp(header, "P4") != 0)
	{
		printf("%s header is not correct\n", input);
		exit(0);
	}

	char* Tmpwidth;
	Tmpwidth = (char *) malloc(1);
	char* Tmpheight;
	Tmpheight = (char *) malloc(1);

	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] != ' ')
		{
			buf[1] = '\0';
			strcat(Tmpwidth, buf);
		} 
		else
			break;
	}
	if(!isDigit(Tmpwidth))
	{
		printf("%s width is not digit\n", input);
		exit(0);
	}
	int width = atoi(Tmpwidth);
	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] != '\n')
		{
			buf[1] = '\0';
			strcat(Tmpheight, buf);
		} 
		else
			break;
	}
	if(!isDigit(Tmpheight))
	{
		printf("%s height is not digit\n", input);
	}
	int height = atoi(Tmpheight);
	int i = 0;
	printf("%s\n", header);
	printf("%d %d\n", width/2, height/2);
	int bitCounter = 0;
	int widthCounter = 0;
	int reseter = 0;
	char result;
	char tmp;
	for(i = 0; i < ceil((double)width/8)*height; i++)
	{
		if(reseter <  ceil((double)width/8))
		{
			if((size = fread(buf,1,1,fp)) == 0)
			{
				printf("File eror %d\n",i);
				exit(0);
			}
			int j = 0;
			for(j = 6; j >= 0; j=j-2)
			{
				tmp = buf[0] & (3 << j);
				tmp >>= j;
				tmp &= 3;
				if(tmp == 3)
					{
					result<<=1;
					result+=1;
					}
				else
				{
					result <<= 1;
					result += 0;
				}
				widthCounter+=2;
				bitCounter++;
				if(widthCounter == width)
					break;
			}

			if(widthCounter == width)
			{
				for(j = bitCounter; j < 8; j++)
				{
					result <<= 1;
				}
				widthCounter = 0;
				printf("%c", result);
				result = 0;
				bitCounter = 0;
				tmp = 0;
			}
			else if(bitCounter == 8)
			{
				printf("%c", result);
				bitCounter = 0;
				result = 0;
				tmp = 0;
			}
			reseter++;
		}
		else
		{
			size = fread(buf,1,1,fp);
			reseter++;
		}
		if(reseter ==  ceil((double)width/8)*2)
		{
			reseter = 0;
		}
	}
	if((size = fread(buf, 1, 1, fp)) != 0 && buf[0] != 0)
	{
		printf("input file error\n");
		exit(0);
	}
}
void decryptKey()
{
	char *buf;
	buf = (char *) malloc(2);
	int size = 0;
	char *header;
	header = (char *) malloc(3);
	while((size = fread(buf, 1, 1, stdin)) > 0)
	{
		if(buf[0] != '\n')
		{
			buf[1] = '\0';
			strcat(header, buf);
		} 
		else
			break;
	}
	if(strcmp(header, "P4") != 0)
	{
		printf("Header is not correct\n");
		exit(0);
	}

	char* Tmpwidth;
	Tmpwidth = (char *) malloc(1 * sizeof(char*));
	char* Tmpheight;
	Tmpheight = (char *) malloc(1* sizeof(char*));

	while((size = fread(buf, 1, 1, stdin)) > 0)
	{
		if(buf[0] != ' ')
		{
			buf[1] = '\0';
			strcat(Tmpwidth, buf);
		} 
		else
			break;
	}
	if(!isDigit(Tmpwidth))
	{
		printf("Width is not digit\n");
		exit(0);
	}
	int width = atoi(Tmpwidth);
	while((size = fread(buf, 1, 1, stdin)) > 0)
	{
		if(buf[0] != '\n')
		{
			buf[1] = '\0';
			strcat(Tmpheight, buf);
		} 
		else
			break;
	}
	if(!isDigit(Tmpheight))
	{
		printf("Height is not digit\n");
	}
	int height = atoi(Tmpheight);
	int i = 0;
	printf("%s\n", header);
	printf("%d %d\n", width/2, height/2);
	int bitCounter = 0;
	int widthCounter = 0;
	int reseter = 0;
	char result;
	char tmp;
	for(i = 0; i < ceil((double)width/8)*height; i++)
	{
		if(reseter <  ceil((double)width/8))
		{
			if((size = fread(buf,1,1,stdin)) == 0)
			{
				printf("File eror %d\n",i);
				exit(0);
			}
			int j = 0;
			for(j = 6; j >= 0; j=j-2)
			{
				tmp = buf[0] & (3 << j);
				tmp >>= j;
				tmp &= 3;
				if(tmp == 3)
					{
					result<<=1;
					result+=1;
					}
				else
				{
					result <<= 1;
					result += 0;
				}
				widthCounter+=2;
				bitCounter++;
				if(widthCounter == width)
					break;
			}

			if(widthCounter == width)
			{
				for(j = bitCounter; j < 8; j++)
				{
					result <<= 1;
				}
				widthCounter = 0;
				printf("%c", result);
				result = 0;
				bitCounter = 0;
				tmp = 0;
			}
			else if(bitCounter == 8)
			{
				printf("%c", result);
				bitCounter = 0;
				result = 0;
				tmp = 0;
			}
			reseter++;
		}
		else
		{
			size = fread(buf,1,1,stdin);
			reseter++;
		}
		if(reseter ==  ceil((double)width/8)*2)
		{
			reseter = 0;
		}
	}
	if((size = fread(buf, 1, 1, stdin)) != 0 && buf[0] != 0)
	{
		printf("input file error\n");
		exit(0);
	}

	free(buf);
	free(header);
	free(Tmpwidth);
	free(Tmpheight);
}

