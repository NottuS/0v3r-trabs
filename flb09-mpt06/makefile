all: diagnostico

diagnostico: diagnostico.o smpl.o rand.o cisj.o
	$(LINK.c) -o $@ -Bstatic diagnostico.o smpl.o rand.o cisj.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

diagnostico.o: diagnostico.c smpl.h
	$(COMPILE.c) -g  diagnostico.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

cisj.o: cisj.c cisj.h
	$(COMPILE.c)  -g cisj.c	

clean:
	$(RM) *.o diagnostico out

