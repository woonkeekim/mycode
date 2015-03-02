#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>

/*
 * Begin code I did not write.
 * The following code was obtained from http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
 */
#define WHITESPACE 64
#define EQUALS     65
#define INVALID    66


const char d[] = {
    66,66,66,66,66,66,66,66,66,64,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,62,66,66,66,63,52,53,
    54,55,56,57,58,59,60,61,66,66,66,65,66,66,66, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9,
    10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,66,66,66,66,66,66,26,27,28,
    29,30,31,32,33,34,35,36,37,38,39,40,41,42,43,44,45,46,47,48,49,50,51,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,66,
    66,66,66,66,66,66
	};
/*
 * End code I did not write.
 */
	
void decode()
{
	unsigned char * buf;
	buf = (unsigned char *) malloc(3);
	uint32_t n = 0;
	uint8_t * ns;
	ns = (uint8_t *) malloc(3);
	int count = 0;
	int eq= 0;
	char input[1];
	while(fgets(input, 2, stdin) != NULL)
	{
		buf[0] = (unsigned char) input[0];
		if(buf[0] == '\n')
			continue;
		if(d[buf[0]] == WHITESPACE)
			continue;
		if(d[buf[0]] == INVALID)
		{
			//printf("Error: invalid input");
			fprintf(stderr, "Error: invalid input");
			exit(0);
		}
		if(d[buf[0]] == EQUALS)
		{
			n += ((uint32_t) 0) << 6 * (3-count);
			eq++;
		}
		else 
			n += ((uint32_t) d[buf[0]]) << 6 * (3-count);
		count++;
		//printf("%d\n", d[buf[0]]);
		if(count == 4)
		{
			int i =0;
			for(i = 0; i < 3-eq; i++)
			{
				ns[i] = (uint8_t)( n >> 8*(2-i));
			}
			for(i = 0; i < 3-eq; i++)
			{
				printf("%c", ns[i]);
			}
			count = 0;
			n = 0;
			buf = (unsigned char *) malloc(4);
		}
	}
}

void decodeFile(char *file)
{
	FILE *fp;
	fp = fopen(file, "r");
	size_t size;
	uint32_t n = 0;
	uint8_t * ns;
	ns = (uint8_t *) malloc(3);
	char * buf;
	buf = (char *) malloc(4);
	int count = 0;
	int eq = 0;
	if(fp == NULL)
	{
		fprintf(stderr, "File %s doesn't exist\n", file);
		exit(0);
	}
	while((size = fread(buf, 1, 1, fp)) > 0)
	{
		if(buf[0] == '\n')	
			continue;
		if(d[(int)buf[0]] ==  WHITESPACE)
			continue;
		if(d[(int)buf[0]] == INVALID)
		{
			fprintf(stderr, "Error: invalid input\n");
			exit(0);
		}
		if(d[(int)buf[0]] == EQUALS && (4-count) > 4)
		{
			fprintf(stderr, "Error: need more bits\n");
			exit(0);
		}
		if(d[(int)buf[0]] == EQUALS)
		{
			n += ((uint32_t) 0) << 6 * (3-count);
			eq++;
		}	
		else 
			n += ((uint32_t) d[(int)buf[0]]) << 6 * (3-count);
		count++;
		if(count == 4)
		{
			int i =0;
			for(i = 0; i < 3-eq; i++)
			{
				ns[i] = (uint8_t)( n >> 8*(2-i));
			}
			for(i = 0; i < 3-eq; i++)
			{
				printf("%c", ns[i]);
			}
			count = 0;
			n = 0;
			buf = (char *) malloc(4);
			eq = 0;
		}
	}
	if(count != 0)
	{
		//printf("Error: not enough bit");
		fprintf(stderr, "Error: not enough bit\n");
		exit(0);
	}
}