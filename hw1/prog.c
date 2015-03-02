#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <string.h>
#include "decode.h"
#include "hexdump.h"
#include "encode.h"

void printUsage()
{
	printf("Wrong format of input\n");
	printf("Usage : hw1 hexdump [file]\n");
	printf("        hw1 enc-base64 [file]\n");
	printf("        hw1 dec-base64 [file]\n");
	printf("[file] is optional");
}
int main(int argc, char** argv)
{
	if(argc == 2)
	{
		if(strcmp(argv[1], "hexdump") == 0)
		{
			hexdump();
		}
		else if(strcmp(argv[1], "enc-base64") == 0)
		{
			encode();
		}
		else if(strcmp(argv[1], "dec-base64") == 0)
		{
			decode();
		}
		else
		{
			printUsage();
			exit(0);
		}
	}
	else if(argc == 3)
	{
		if(strcmp(argv[1], "hexdump") == 0)
		{
			hexdumpFile(argv[2]);
		}
		else if(strcmp(argv[1], "enc-base64") == 0)
		{
			encodeFile(argv[2]);
		}
		else if(strcmp(argv[1], "dec-base64") == 0)
		{
			decodeFile(argv[2]);
		}
		else
		{
			printUsage();
			exit(0);
		}
	}
	else
	{
		printUsage();
		exit(0); 
	}
return 0;
}
