all: Trabalho_Distribuida

Trabalho_Distribuida: Trabalho_Distribuida.o smpl.o rand.o funcao_cis.o
	$(LINK.c) -o $@ -Bstatic Trabalho_Distribuida.o smpl.o rand.o funcao_cis.o -lm

smpl.o: smpl.c smpl.h
	$(COMPILE.c)  -g smpl.c

funcao_cis.o: funcao_cis.c funcao_cis.h
	$(COMPILE.c) -g funcao_cis.c

Trabalho_Distribuida.o: Trabalho_Distribuida.c smpl.h funcao_cis.h
	$(COMPILE.c) -g  Trabalho_Distribuida.c

rand.o: rand.c
	$(COMPILE.c) -g rand.c

clean:
	$(RM) *.o Trabalho_Distribuida relat saida
