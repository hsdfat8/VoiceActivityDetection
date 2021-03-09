CC = gcc

CFLAGS = -Wall -O4

LOBJECTS = basicop2.o log2.o oper_32b.o pow2.o r_fft.o vad2.o
       
LSRC = $(LOBJECTS:.o=.c)

LIBS = -lm

libvad.a: $(LOBJECTS)
	ar cr libvad.a $(LOBJECTS)

clean:
	del -f *.o *.a

depend:
	makedepend -- $(CFLAGS) -- $(LSRC)


# DO NOT DELETE
