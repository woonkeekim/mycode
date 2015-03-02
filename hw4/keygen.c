#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <openssl/md5.h>  /* please read this */
void stream(char *input, int limit, unsigned char result[])
{
    unsigned char md5_buf[MD5_DIGEST_LENGTH];
    unsigned char *p = (unsigned char*) input;
    int len=strlen((char *)input)+2+MD5_DIGEST_LENGTH;
    unsigned char s[len+1];
          
    int i=0;
    int k = 0;
    int j = 0;

    MD5(p, strlen((char *)p), md5_buf);

    for (;;) {
        sprintf((char *)&s[MD5_DIGEST_LENGTH], "%02d%s", i, p);
        memcpy(s, md5_buf, MD5_DIGEST_LENGTH);
        MD5(s, len, md5_buf);
        for(j = 0; k < limit && j < 8; j++)
        {
                result[k] = md5_buf[j];
                k++;
        }
        if(k == limit)
          break;

        if (++i == 100) i = 0;
    }

    free(s);
}
void keygen(char *pphrase, int period)
{
	unsigned char result[period*26*4*2];
	stream(pphrase, period*26*4*2, result);
	int i = 0;
	int L = 0;
	int j = 0;
	int arrayCounter = 0;
	char *state = "abcdefghijklmnopqrstuvwxyz";
	for(i = 0; i < period; i++)
	{
		char *state_tmp = malloc(sizeof(char *)*26);
		strcpy(state_tmp, state);
		for(j = 0; j < 2; j++)
		{
			L = 26;
			while (L > 1)
			{
				unsigned int X;
				int k = 0;
				for(k = 0; k < 4; k++)
				{
					X <<= 8;
					X += (unsigned int) result[arrayCounter];
					arrayCounter++;
				}
				if( (X % L)  != (L - 1))
				{
					char tmp = state_tmp[X % L];
					state_tmp[X % L] = state_tmp[L - 1];
					state_tmp[L - 1] = tmp;
				}
				L--; 
			}
		}
		printf("%s\n", state_tmp);
		free(state_tmp);
	}
}
