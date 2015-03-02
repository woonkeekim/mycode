#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "stream.h"

//printf("hw2 encrypt -p=pphrase -out=name [pbmfile]\n");
//void stream(char *input, int limit)

int isDigit(char *input)
{
        int i =0;
        for(i = 0; i < strlen(input); i++)
        {
                if((input[i] - '0') > 9 || (input[i] - '0') < 0)
                {       
                        return 0;
                }
        }
        return 1;
}

void encrypt(char *pphrase, char* output1, char* output2, char* input)
{
	char *header;
	header = (char *) malloc(1);
	char *buf;
	buf = (char *) malloc(2);
	FILE *fp;
	fp  = fopen(input, "r");
	if(fp == NULL)
	{
		printf("file doesnt exist");
		exit(0);
	}
	int size = 0;
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
	   printf("this is not digit");
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
	   printf("this is not digit");
	   exit(0);
	}
	int height = atoi(Tmpheight);

	FILE *file1;
	FILE *file2;
	file1 = fopen(output1, "w");
	file2 = fopen(output2, "w");
	if(file1 == NULL)
	{
		printf("%s cannot be created", output1);
		exit(0);
	}
	if(file2 == NULL)
	{
		printf("%s cnnot be created", output2);
		exit(0);
	}
	fprintf(file1, "%s\n", header);
	fprintf(file2, "%s\n", header);
	fprintf(file1, "%d %d\n", width*2, height*2);
	fprintf(file2, "%d %d\n", width*2, height*2);
/*
	unsigned char* test;
	test = malloc((width*height) * sizeof (unsigned char*));
	test = stream(pphrase, width*height);
*/
	unsigned char test[width*height];
        stream(pphrase, width*height, test);


	int i = 0;
	int j = 0;
	int widthTracker = 0;
	int keyTracker = 7;
	int keyCounter = 0;
	int outputCounter = 0;
	char out1[(int)ceil((double)width/4)];
	char out2[(int)ceil((double)width/4)];
	char out11[(int)ceil((double)width/4)];
	char out21[(int)ceil((double)width/4)];
	int arrayTracker = 0;
	int newWidthCounter = 0;
	for(i = 0; i < ceil((double)width/8)*height; i++)
	{
		if((size = fread(buf, 1, 1, fp)) <= 0)
		{
			printf("error during reading a file size is %d\n", i);
			break;
		}
		else
		{
			j = 7;
		}
		for(; j >= 0; j--)
		{
			char inputPixel = buf[0] & (1<<j);
			inputPixel >>= j;
			inputPixel = inputPixel & 1;
			char key = test[keyCounter] & (1<<keyTracker);
			key >>= keyTracker;
			key = key & 1;
			keyTracker--;

			if(inputPixel == 0)
			{
				if(key == 0)
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 2;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 1;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 2;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 1;
				}
				else
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 1;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 2;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 1;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 2;
				}
				outputCounter += 2;
				newWidthCounter +=2;

			}
			else
			{
				if(key == 0)
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 2;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 1;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 1;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 2;
				}
				else
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 1;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 2;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 2;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 1;
				}
				outputCounter += 2;
				newWidthCounter +=2;

			}
			if(newWidthCounter == width* 2)
			{
				out1[arrayTracker] <<= (8-outputCounter);
				out11[arrayTracker] <<= (8-outputCounter);
				out2[arrayTracker] <<= (8-outputCounter);
				out21[arrayTracker] <<= (8-outputCounter);
				outputCounter = 8;
				newWidthCounter = 0;
			}
			if(outputCounter == 8)
			{

				arrayTracker++;
				outputCounter = 0;
				if(arrayTracker == (int)ceil((double)width/4)) 
				{
					int m = 0;
					for(m = 0; m < arrayTracker; m++)
					{
						fprintf(file1, "%c", out1[m]);
						fprintf(file2, "%c", out2[m]);
						out1[m] = 0;
						out2[m] = 0;
					}
					for(m = 0; m < arrayTracker; m++)
					{
						fprintf(file1, "%c", out11[m]);
						fprintf(file2, "%c", out21[m]);
						out11[m] = 0;
						out21[m] = 0;
					}
					arrayTracker = 0;
				}
			}


			if(keyTracker == -1)
			{
				keyTracker = 7;
				keyCounter++;
			}

			widthTracker++;
			if(widthTracker == width)
			{
				widthTracker = 0;
				break;
			}
		}
	}
	fclose(fp);
	fclose(file1);
	fclose(file2);
	free(test);
	if((size = fread(buf, 1, 1, fp)) != 0 && buf[0] != 0)
	{
		printf("input file error\n");
		exit(0);
	}

}
void encryptKey(char *pphrase, char* output1, char* output2)
{
	FILE * file1;
	FILE * file2;

	//header test
	char* header;
	header = (char *)malloc(3 * sizeof(char *));
	int size = 0;
	char *buf;
	buf = (char *) malloc(1);

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
		printf("Header error\n");	
		exit(0);
	}
	//width and height
	buf = (char *) malloc(1);
        char* Tmpwidth;
        Tmpwidth = (char *) malloc(1);
        char* Tmpheight;
        Tmpheight = (char *) malloc(1);
	while(fgets(buf, 2, stdin) != NULL)
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
		printf("Width value error\n");
		exit(0);
	}
	int width = atoi(Tmpwidth);

	while(fgets(buf, 2, stdin) != NULL)
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
		printf("height value error\n");
		exit(0);
	}
	int height = atoi(Tmpheight);

	//file out
	file1 = fopen(output1, "w");
	file2 = fopen(output2, "w");
	
	if(file1 == NULL)
	{
		printf("%s cannot be created", output1);
		exit(0);
	}
	if(file2 == NULL)
	{
		printf("%s cnnot be created", output2);
		exit(0);
	}
	fprintf(file1, "%s\n", header);
	fprintf(file2, "%s\n", header);
	fprintf(file1, "%d %d\n", width*2, height*2);
	fprintf(file2, "%d %d\n", width*2, height*2);
	
	unsigned char test[width*height];
        stream(pphrase, width*height, test);



	int i = 0;
	int j = 0;
	int widthTracker = 0;
	int keyTracker = 7;
	int keyCounter = 0;
	int outputCounter = 0;
	char out1[(int)ceil((double)width/4)];
	char out2[(int)ceil((double)width/4)];
	char out11[(int)ceil((double)width/4)];
	char out21[(int)ceil((double)width/4)];
	int arrayTracker = 0;
	int newWidthCounter = 0;

	for(i = 0; i < ceil((double)width/8)*height; i++)
	{
		if((size = fread(buf, 1, 1, stdin)) <= 0)
		{
			printf("error during reading a file size is %d\n", i);
			break;
		}
		else
		{
			j = 7;
		}
		for(; j >= 0; j--)
		{
			char inputPixel = buf[0] & (1<<j);
			inputPixel >>= j;
			inputPixel = inputPixel & 1;
			char key = test[keyCounter] & (1<<keyTracker);
			key >>= keyTracker;
			key = key & 1;
			keyTracker--;

			if(inputPixel == 0)
			{
				if(key == 0)
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 2;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 1;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 2;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 1;
				}
				else
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 1;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 2;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 1;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 2;
				}
				outputCounter += 2;
				newWidthCounter +=2;

			}
			else
			{
				if(key == 0)
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 2;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 1;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 1;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 2;
				}
				else
				{
					out1[arrayTracker] <<= 2;
					out1[arrayTracker] |= 1;
					out11[arrayTracker] <<= 2;
					out11[arrayTracker] |= 2;
					out2[arrayTracker] <<= 2;
					out2[arrayTracker] |= 2;
					out21[arrayTracker] <<= 2;
					out21[arrayTracker] |= 1;
				}
				outputCounter += 2;
				newWidthCounter +=2;

			}
			if(newWidthCounter == width* 2)
			{
				out1[arrayTracker] <<= (8-outputCounter);
				out11[arrayTracker] <<= (8-outputCounter);
				out2[arrayTracker] <<= (8-outputCounter);
				out21[arrayTracker] <<= (8-outputCounter);
				outputCounter = 8;
				newWidthCounter = 0;
			}
			if(outputCounter == 8)
			{

				arrayTracker++;
				outputCounter = 0;
				if(arrayTracker == (int)ceil((double)width/4)) 
				{
					int m = 0;
					for(m = 0; m < arrayTracker; m++)
					{
						fprintf(file1, "%c", out1[m]);
						fprintf(file2, "%c", out2[m]);
						out1[m] = 0;
						out2[m] = 0;
					}
					for(m = 0; m < arrayTracker; m++)
					{
						fprintf(file1, "%c", out11[m]);
						fprintf(file2, "%c", out21[m]);
						out11[m] = 0;
						out21[m] = 0;
					}
					arrayTracker = 0;
				}
			}


			if(keyTracker == -1)
			{
				keyTracker = 7;
				keyCounter++;
			}

			widthTracker++;
			if(widthTracker == width)
			{
				widthTracker = 0;
				break;
			}
		}
	}
	if((size = fread(buf, 1, 1, stdin)) != 0 && buf[0] != 0)
	{
		printf("input file error\n");
		exit(0);
	}


	fclose(file1);
	fclose(file2);

}
