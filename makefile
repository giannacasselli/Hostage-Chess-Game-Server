CC = clang
CFLAGS = -Wall -std=c99 -pedantic

all:  myprog

clean:  
	rm -f *.o *.so hclib_wrap.c hclib.py myprog 

hclib.o:  hclib.c hclib.h
	$(CC) $(CFLAGS) -c hclib.c -fPIC -o hclib.o

libhclib.so: hclib.o
	$(CC) -shared -o libhclib.so hclib.o -lm

main.o:  main.c hclib.h
	$(CC) $(CFLAGS) -c main.c -o main.o

hclib_wrap.c: hclib.i
	swig -python hclib.i

hclib_wrap.o: hclib_wrap.c
	$(CC) $(CFLAGS) -c hclib_wrap.c -I/usr/include/python3.11/ -fPIC -o hclib_wrap.o

_hclib.so: hclib_wrap.o libhclib.so
	$(CC) $(CFLAGS) -shared hclib_wrap.o -L. -L/usr/lib/python3.11/config-3.11-x86_64-linux-gnu -lpython3.11 -lhclib -o _hclib.so

myprog:  main.o libhclib.so hclib_wrap.o _hclib.so
	$(CC) main.o -L. -lhclib -o myprog

