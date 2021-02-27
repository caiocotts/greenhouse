ghc:  ghc.o ghcontrol.o
	gcc -g -o ghc ghc.o ghcontrol.o

ghc.o: ghc.c ghcontrol.h
	gcc -g -c ghc.c

ghcontrol.o: ghcontrol.c ghcontrol.h
	gcc -g -c ghcontrol.c

clean:
	touch *
	rm -f *.o ghc stamp.txt