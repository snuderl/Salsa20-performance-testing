compile:
	gcc -o ecrypt.o -c ecrypt.c
	gcc -o ecrypt-ref.o -c salsa20-ref.c
	gcc -o ecrypt-regs.o -c salsa20-regs.c
	gcc -o ecrypt-merged.o -c salsa20-merged.c

test: 	compile
	gcc bigtest.c -lcrypto ecrypt.o

test1: 	compile
		gcc -osalsa20.out test.c -lcrypto ecrypt.o
		gcc -osalsa20-refs.out test.c -lcrypto ecrypt-ref.o
		gcc -osalsa20-regs.out test.c -lcrypto ecrypt-regs.o
		gcc -osalsa20-merged.out test.c -lcrypto ecrypt-merged.o
		mcs Salsa-csharp-test.cs Salsa20.cs

run:
		gcc -o ecrypt.o -c ecrypt.c
		gcc -osalsa20.out test.c -lcrypto ecrypt.o
		./salsa20.out

git:
		git pull


runall: test1
		./salsa20.out
		./salsa20-refs.out
		./salsa20-regs.out
		./salsa20-merged.out
		mono Salsa-csharp-test.exe


make: git runall

clean:
	rm -rf *o
	rm -rf *out
	rm -rf *exe
