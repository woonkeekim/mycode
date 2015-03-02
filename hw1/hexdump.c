#include <stdlib.h>
#include <stdio.h>

void printable(unsigned char c, unsigned char *print)
{
	if(c <= 0x1f || c == 0x7f)
		*print = '.';
	else if(c >= 0x20 && c <= 0x7e)
		*print = (char) c;
	else
		*print = '~';
}
void hexdump()
{
	int read = 0;
	unsigned char * input;
	input = (unsigned char *) malloc(1);
	char buf[1];
	unsigned char *output;
	output = (unsigned char *) malloc(16);
	int position = 0;
	while(fgets(buf, 2, stdin) != NULL)
	{
		
		input[0] = (unsigned char) buf[0];
		if(read == 0)
			printf("%06x: ", position);
		printf("%02x ", input[0]);
		printable(input[0], &output[read]);
		if((read + 1) % 8 == 0)
			printf(" ");
		read++;
		if(read == 16)
		{
			int i = 0;
			for(i = 0; i < read; i++)
				printf("%c", output[i]);
			printf("\n");
			position = position+16;
			input = (unsigned char *) malloc(1);
			output = (unsigned char *) malloc(16);
			read = 0;
			free(input);
			free(output);
		}
	}
	int i = 0;
	for(i = read; i < 16; i++)
	{
		printf("-- ");
		if((i + 1) % 8 == 0)
			printf(" ");
	}
	for(i = 0; i < read; i++)
	{
		printf("%c", output[i]);
	}
	for(i = read; i < 16; i++)
	{
		printf(" ");
	}
	printf("\n");
	exit(1);
}
void hexdumpFile(char *file)
{
	FILE *f;
	f = fopen(file, "r");
	size_t size;
	unsigned char *input;
	input = (unsigned char *) malloc(16);
	unsigned char *output;
	output = (unsigned char *) malloc(16);
	int position = 0;
	if(f == NULL)
	{
		fprintf(stderr, "File %s doesnt exist\n", file);
		exit(1);
	}
	else
	{
		while( (size = fread(input, 1, 16, f)) > 0)
		{
			int i = 0;
			fprintf(stdout,"%06x: ", position);
			for(i = 0; i < size; i++)
			{
				fprintf(stdout,"%02x ", input[i]);
				printable(input[i], &output[i]);
				if((i+1) % 8 == 0)
					printf(" ");
			}
			if(size != 16)
			{
				for(i = size; i < 16; i++)
				{
					printf("-- ");
					if((i+1) % 8 == 0)
						printf(" ");
				}
			}
			for(i = 0; i < size; i++)
			{
				fprintf(stdout,"%c", output[i]);
			}
			for(i = size; i < 16; i++)
			{
				printf("%c", ' ');
			}
			printf("\n");
			position = position + 16;
			input = (unsigned char *) malloc(16);
			output = (unsigned char *) malloc(16);
		}
	}
	fclose(f);
}
