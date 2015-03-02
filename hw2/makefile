hw2:
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include stream.c 
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include encrypt.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include merge.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include decrypt.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include prog.c
	gcc -o hw2 *.o -L /home/scf-22/csci551b/openssl/lib  -lcrypto -lm
clean:
	/bin/rm -rf *.o
	/bin/rm -rf *.pbm
	/bin/rm -rf *.stream
	/bin/rm -rf hw2
