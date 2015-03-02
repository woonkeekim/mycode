#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
/*
 * Begin code I did not write.
 * The following code was obtained from http://en.wikibooks.org/wiki/Algorithm_Implementation/Miscellaneous/Base64
 */
const char base64Table[] = {"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"};
/*
     * End code I did not write.
     */
void encode()
{
	unsigned char * buf;
	buf = (unsigned char *) malloc(3);
	uint32_t n = 0;
	uint8_t * ns;
	ns = (uint8_t *) malloc(3);
	int counter = 0;
	int size = 0;
	char input[1];
	
	while(fgets(input, 2, stdin) != NULL)
	{
		buf[0] = (unsigned char) input[0];
		n += ((uint32_t) buf[0]) << 8 * (2-size);
		
		size++;
		counter++;
		if(size == 3)
		{
			int i = 0;
			for(i = 0; i < size+1; i++)
			{
				ns[i] = (uint8_t)( n >> 6*(3-i)) & 63;
			}
			for(i = 0; i < size+1; i++)
			{
				printf("%c", base64Table[ns[i]]);
			}
			free(buf);
			free(ns);
			ns = (uint8_t *) malloc(3);
			buf = (unsigned char *)malloc(3);
			n = 0;
			size = 0;
			counter++;
		}
		if(counter % 64 == 0)
		{
			counter = 0;
			printf("\n");
		}
	}
	if(size != 0)
	{
		int i = 0;
		for(i = 0; i < size+1; i++)
		{
			ns[i] = (uint8_t)( n >> 6*(3-i)) & 63;
		}
		for(i = 0; i < size+1; i++)
		{
			printf("%c", base64Table[ns[i]]);
		}
		for(;size < 3; size++)
			printf("=");
	}
	if(counter % 64 != 0)
		printf("\n");
}
void encodeFile(char *file)
{
	FILE *fp;
	unsigned char * buf;
	buf = (unsigned char *) malloc(3);
	uint32_t n = 0;
	uint8_t * ns;
	ns = (uint8_t *) malloc(3);
	fp = fopen(file, "r");
	if(fp == NULL)
	{
		fprintf(stderr, "File %s doesnt exist\n", file);
		exit(1);
	}
	size_t size;
	int counter = 0;
	while( (size = fread(buf, 1, 3, fp)) > 0)
	{
		int i = 0;
		for(i = 0; i < size; i++)
		{
			n += ((uint32_t) buf[i]) << 8 * (2-i);
		}
		for(; i < 3; i++)
		{
			n += 0 << 8 * (2-i);
		}
		
		for(i = 0; i < size+1; i++)
		{
			ns[i] = (uint8_t)( n >> 6*(3-i)) & 63;
		}
		
		for(i = 0; i < size+1; i++)
		{
			printf("%c", base64Table[ns[i]]);
		}
		free(buf);
		free(ns);
		ns = (uint8_t *) malloc(3);
		buf = (unsigned char *)malloc(3);
		n = 0;
		if(size < 3)
		{	
			for(;size < 3; size++)
				printf("=");
		}
		counter += 4;
		if((counter % 64) == 0)
		{
			printf("\n");
			counter = 0;
		}
	}
	if(counter % 64 != 0)
		printf("\n");
	fclose(fp);
}
