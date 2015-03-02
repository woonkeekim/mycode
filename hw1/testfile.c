set srcdir=~csci531/public/cs531/hw1
/bin/rm -f f*.hex f??.hex
                foreach f (0 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
                    echo "===> $srcdir/f$f"
                    ./hw1 hexdump $srcdir/f$f > f$f.hex
                    diff $srcdir/f$f.hex f$f.hex
                end
/bin/rm -f f*.hex f??.hex
                foreach f (0 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
                    echo "===> $srcdir/f$f"
                    cat $srcdir/f$f | ./hw1 hexdump > f$f.hex
                    diff $srcdir/f$f.hex f$f.hex
                end
 
 set srcdir=~csci531/public/cs531/hw1
 /bin/rm -f f*.b64
                foreach f (0 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
                    echo "===> $srcdir/f$f"
                    ./hw1 enc-base64 $srcdir/f$f > f$f.b64
                    diff $srcdir/f$f.b64 f$f.b64
                end
 /bin/rm -f f*.b64
                foreach f (0 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
                    echo "===> $srcdir/f$f"
                    cat $srcdir/f$f | ./hw1 enc-base64 > f$f.b64
                    diff $srcdir/f$f.b64 f$f.b64
                end
				
 set srcdir=~csci531/public/cs531/hw1
 /bin/rm -f f*.dat
                foreach f (1 2 3 4 5 6 7 8 9 10 11 12 13 14)
                    echo "===> $srcdir/f$f"
                    ./hw1 dec-base64 $srcdir/f$f.b64 > f$f.dat
                    diff $srcdir/f$f f$f.dat
                end
			
				
/bin/rm -f f*.dat
                foreach f (0 1 2 3 4 5 6 7 8 9 10 11 12 13 14)
                    echo "===> $srcdir/f$f"
                    cat $srcdir/f$f.b64 | ./hw1 dec-base64 > f$f.dat
                    diff $srcdir/f$f f$f.dat
                end

/bin/rm -f f*.dat
                foreach f (100 101)
                    echo "===> $srcdir/f$f"
                    ./hw1 dec-base64 $srcdir/f$f.b64 > f$f.dat
                    diff $srcdir/f$f f$f.dat
                end
				
/bin/rm -f f*.dat f*.dat
                foreach f (100 101)
                    echo "===> $srcdir/f$f"
                    cat $srcdir/f$f.b64 | ./hw1 dec-base64 > f$f.dat
                    diff $srcdir/f$f f$f.dat
                end
 

 /bin/rm -f f?.dat
                foreach f (0 1 2 3 4 5 6 7 8 9)
                    echo "===> $srcdir/f$f"
                    ./hw1 enc-base64 $srcdir/f$f | ./hw1 dec-base64 > f$f.dat
                    diff $srcdir/f$f f$f.dat
                end