#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "encrypt.h"
void merge(char* input1, char* input2)
{
	FILE * file1;
	FILE * file2;
	file1 = fopen(input1, "r");
	file2 = fopen(input2, "r");
	if(file1==NULL)
	{
		printf("file %s does not exist\n", input1);
		exit(0);
	}
	if(file2==NULL)
	{
		printf("file %s does not exist\n", input2);
		exit(0);
	}

	char *buf;
	buf = (char *) malloc(2 * sizeof(char*));
	int size = 0;
	char *header;
	header = (char *) malloc(3 * sizeof(char*));
	while((size = fread(buf, 1, 1, file1)) > 0)
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
		printf("%s header is not correct\n", input1);
		exit(0);
	}
	strcpy(header, "");

	while((size = fread(buf, 1, 1, file2)) > 0)
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
		printf("%s header is not correct\n", input2);
		exit(0);
	}

	char* Tmpwidth;
        Tmpwidth = (char *) malloc(10 * sizeof(char*));

        char* Tmpheight;
        Tmpheight = (char *) malloc(10 * sizeof(char*));

        while((size = fread(buf, 1, 1, file1)) > 0)
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
           printf("%s width is not digit\n", input1);
           exit(0);
        }
        int width1 = atoi(Tmpwidth);
        while((size = fread(buf, 1, 1, file1)) > 0)
        {
           if(buf[0] != '\n')
                 strcat(Tmpheight, buf); 
           else
                break;
        }
        if(!isDigit(Tmpheight))
	{
		printf("%s height is not digit\n", input1);
	}
	int height1 = atoi(Tmpheight);

	strcpy(Tmpwidth, "");
	strcpy(Tmpheight, "");
        while((size = fread(buf, 1, 1, file2)) > 0)
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
           printf("%s width is not digit\n", input2);
           exit(0);
        }

        int width2 = atoi(Tmpwidth);
        while((size = fread(buf, 1, 1, file2)) > 0)
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
		printf("%s height is not digit\n", input2);
		exit(0);
	}
	int height2 = atoi(Tmpheight);

	if(width1 != width2)
	{
		printf("width of two files are different\n");
		exit(0);
	}
	if(height1 != height2)
	{
		printf("height of two files are different\n");
		exit(0);
	}

	int i = 0;
	char *read1;
	char *read2;
	read1 = (char *) malloc(1 * sizeof(char*));
	read2 = (char *) malloc(1 * sizeof(char*));
	char bit1 = 0;
	char bit2 = 0;

	printf("%s\n", header);
	printf("%d %d\n", width1, height1);

	for(i = 0; i < ceil((double)width1/8)*height1; i++)
        {
                if((size = fread(read1, 1, 1, file1)) <= 0)
                {
			printf("%s has errors in file\n", input1);
			exit(0);
		}
                if((size = fread(read2, 1, 1, file2)) <= 0)
                {
			printf("%s has error in file\n", input2);
			exit(0);
		}
		int j = 7;
		unsigned char output = 0;
		for(; j >=0; j--)
		{
			bit1 = read1[0] & (1<<j);
			bit1 >>= j;
			bit1 = bit1 & 1;
			bit2 = read2[0] & (1<<j);
			bit2 >>= j;
			bit2 = bit2 & 1;
			if(bit1 == 0 && bit2 == 0)
			{
				output <<= 1;
				output += 0;
			}
			else
			{
				output <<= 1;
				output += 1;
			}
		}
		//fprintf(merge,"%c", output);
		printf("%c", output); 
	}
	if((size = fread(buf, 1, 1, file1)) != 0 && buf[0] != 0)
	{
		printf("input file error\n");
		exit(0);
	}
	if((size = fread(buf, 1, 1, file2)) != 0 && buf[0] != 0)
	{
		printf("input file error\n");
		exit(0);
	}

	free(file1);
	free(file2);
	free(read1);
	free(read2);
	free(header);
}

