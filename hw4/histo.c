#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
void sortResult(int result[][2])
{
	int i = 0;
	int j = 0;
	for(i = 0; i < 25; i++)
	{
		for(j = i+1; j < 26; j++)
		{
			if(result[i][1] < result[j][1])
			{
				int header = result[j][0];
				int tail = result[j][1];
				result[j][0] = result[i][0];
				result[j][1] = result[i][1];
				result[i][0] = header;
				result[i][1] = tail;
			}
			else if(result[i][1] == result[j][1])
			{
				if(result[i][0] > result[j][0])
				{
					int header = result[j][0];
					int tail = result[j][1];
					result[j][0] = result[i][0];
					result[j][1] = result[i][1];
					result[i][0] = header;
					result[i][1] = tail;
				}
			}
		}
	}
}
void histoFromKey(int t, int i)
{
        int count[26][2];
        int k = 0; 
        for(k = 0; k < 26; k++) 
        {
                count[k][0] = k;
                count[k][1] = 0;
        }
        int total = 0;
        char *buf;
        int size = 0;
        buf = (char *) malloc(sizeof(char*) * t);
        while((size = fread(buf, t, 1, stdin)) != 0)
        {       
                if(buf[i-1] >= 'a' && buf[i-1] <= 'z')
                {       
                        total++;
                        count[buf[i-1]-'a'][1]+=1;
                }
        }
        printf("L=%d\n", total);
        sortResult(count);
        int j = 0;
        for(j = 0; j < 26; j++)
        {
                printf("%c: %d (%.2f%%)\n",  (char)(count[j][0]+'a'), count[j][1], (double)count[j][1]/total*100);
        }

}
void histo(int t, int i, char* input)
{
	FILE *fp;
	fp=fopen(input, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "File %s cannot be opened", input);
		exit(0);
	}
	int count[26][2];
	int k = 0;
	for(k = 0; k < 26; k++)
	{
		count[k][0] = k;
		count[k][1] = 0;
	}
	int total = 0;
	char *buf;
	int size = 0;
	buf = (char *) malloc(sizeof(char*) * t);
	while((size = fread(buf, t, 1, fp)) != 0)
	{
		if(buf[i-1] >= 'a' && buf[i-1] <= 'z')
		{
			total++;
			count[buf[i-1]-'a'][1]+=1;
		}
	}
	fclose(fp);
	printf("L=%d\n", total);
	sortResult(count);
	int j = 0;
	for(j = 0; j < 26; j++)
	{
		printf("%c: %d (%.2f%%)\n",  (char)(count[j][0]+'a'), count[j][1], (double)count[j][1]/total*100);
	}

}
