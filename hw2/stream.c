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


