hw4:
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include keygen.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include crypt.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include invkey.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include histo.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include solve.c
	gcc -c -g -Wall -I /home/scf-22/csci551b/openssl/include prog.c
	gcc -o hw4 *.o -L /home/scf-22/csci551b/openssl/lib  -lcrypto -lm
clean:
	rm -rf *.o
	rm hw4
