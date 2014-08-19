base_de_regras_e(
[
[ [pernas, 2], ->, 1.0, [especie, humana] ],
[ [pernas, 4], ->, -1.0, [especie, humana] ],
[ [pernas, 4], ->, 0.5, [especie, cao] ],
[ [altura, baixa], ->, 0.5, [especie, cao] ],
[ [altura, alta], ->, 0.5, [especie, humana] ]
]).

base_de_regras_c(
[
[ [idade, jovem], [especie, humana], [sexo, masculino], -> , 1.0, [chamado, menino] ],
[ [idade, jovem], [especie, humana], [sexo, feminino], -> , 1.0, [chamado, menina] ],
[ [idade, adulto], [especie, humana], [sexo, masculino], -> , 1.0, [chamado, homem] ],
[ [idade, adulto], [especie, humana], [sexo, feminino], -> , 1.0, [chamado, mulher] ],
[ [idade, jovem], [especie, cao], ->, 1.0, [chamado, filhote] ],
[ [idade, adulto], [especie, cao], [sexo, feminino], ->, 1.0, [chamado, cadela] ],
[ [idade, adulto], [especie, cao], ->, 0.5, [chamado, cao] ]
]).

%BF = [[altura, [baixa, -0.2]], [especie, [humana, 1.0]], [pernas, [2, 1.0]]] ?

flexa(->).

imprime_desc(1,Atr):-
	write('>> Descobrindo sobre '), 
	write(Atr), 
	write(.),nl,!.

imprime_desc(_,Atr).
	
imprime_abandonando(1,Atr):-
	write('>> Abandonando esta regra para '),
	write(Atr), 
	write(.),nl,!.

imprime_abandonando(_,Atr).

imprime_nova_exatidao(1,Atr, Valor,NovaExatidao):-
	write('>> A exatidao de '),
	write( Atr ),write(' '),write('='),
	write(' '),write(Valor), write(' '), 
	write('agora eh '),write(NovaExatidao),
	write(.),nl,!.

imprime_nova_exatidao(_,Atr, Valor,NovaExatidao).

imprime_tentando_regra(1, Regra):-
	write('>> Tentando a regra '),
	imprime_regra(1,Regra),
	write(.),nl,!.

imprime_tentando_regra(_, Regra).

imprime_porque(porque, Regra):-
	write('Eu estou tentando usar a regra:'),nl,write('\t'),
	imprime_regra(1,Regra),!.

imprime_porque(_, Regra).	

imprime_regra(1, Regra):-
	write('['), imprime_regra(2, Regra),
	write('].'),!.

imprime_atributo(X):-
	write('['),
	atributo(X, Atributo, Valor),
	write(Atributo),write(' '), 
	write(Valor), write(']').

separa([X|L]):- 
	imprime_atributo(X).

imprime_regra(2, [->,X|L]):-
	write(' '), write(->), write(' '),
	write(X), write(' '), separa(L),
	!.
		
imprime_regra(2, [X|L]):-
	imprime_atributo(X), 
	imprime_regra(2, L),!.
	
imprime_regra(_, Regra).

limiar_de_exatidao(L):-
	L is 0.2.

base_de_fatos(BF).

min(X,Y,Min):-
	Min is X,
	abs(X) < abs(Y);
	Min is Y.

calcula(Antiga, Nova, Resultante, 3):-
	min(Antiga, Nova, Min), 
	Resultante is ((Antiga + Nova) / (1 - Min)).

calcula(Antiga, Nova, Resultante, 2):-  	
	Antiga < 0,
	Nova < 0, 
	Resultante is (Antiga + Nova + (Antiga * Nova));
	calcula(Antiga, Nova, Resultante, 3).

calcula(Antiga, Nova, Resultante, 1):- 
	Antiga > 0, 
	Nova > 0,
	Resultante is (Antiga + Nova - (Antiga * Nova));
	calcula(Antiga, Nova, Resultante, 2).

atributo([X,L|_], Atributo,Valor):-
	Atributo = X, 
	Valor = L.

separa([X|L], Especie):-
	atributo(X, Aux, Especie).
	
exat_especie([X|L], Exatidao, Especie):-
	Exatidao is X, 
	separa(L, Especie).
	
busca_exatidao([X|L], Exatidao, Especie):-
	flexa(X), exat_especie(L, Exatidao, Especie);
	busca_exatidao(L, Exatidao, Especie).

busca_atributo_e([X|L], Atr, Valor):-
	\+flexa(X),
	atributo(X, Atributo, Valor), 
	Atributo == Atr; 
	busca_atributo_e(L, Atr, Valor).	
	
busca_regra_e([X|L], Atr, ValorDigitado, Valor, Exatidao, Especie, Regra):-
	Regra = X,
	%BF is L,
	busca_atributo_e(X, Atr, Valor), 
	ValorDigitado == Valor, 
	busca_exatidao(X, Exatidao, Especie);
	busca_regra_e(L, Atr,  ValorDigitado, Valor, Exatidao, Especie, Regra).

imprime_e(Valor, Exatidao, Especie):-	
	nl, write(Valor),
	nl, write(Exatidao), 
	nl, write(Especie), nl.
	
add(A,B):-
	B = [A,B].

monitoramento(M):-
	retract(monitoramento),
	M is 1;true.

pergunta_pernas(P, Regra ):-
	write('Me informe sobre pernas ?'), 
	read(P), 
	P \= porque; 
	imprime_porque(porque, Regra),
	pergunta_pernas(P, Regra ).

pergunta_exatidao(E1, Regra):-
	write('Qual eh o seu grau de exatidao (um numero entre -1 e 1) ?'),
	read(E1),
	E1 \= porque; 
	imprime_porque(porque, Regra),
	pergunta_exatidao(P, Regra ).

pergunta_altura(P, Regra ):-
	write('Me informe sobre altura ?'), 
	read(P), 
	P \= porque; 
	imprime_porque(porque, Regra),
	pergunta_pernas(P, Regra ).
	
diagnostico(especie):-
	M is 0,
	monitoramento(M),
	imprime_desc(M,especie),
	base_de_regras_e(BR),
	limiar_de_exatidao(L),
	busca_regra_e(BR, altura, baixa,Valor, Exatidao, Especie, Regra),
	imprime_desc(M,pernas), 
	pergunta_pernas( P, Regra ),
	%busca_regra_e(BR, pernas, P,Valor, Exatidao, Especie),
	%add([pernas,[Valor,P]]),
	calcula(0,Exatidao, Resultante,1),
	%add()
	/*Tupla = pernas,
	findall(Tupla,true,BF),
	write(BF),
	add(especie,BF),*/
	imprime_e(Valor, Exatidao, Especie),
	pergunta_exatidao(E1, Regra),
	imprime_desc(M,pernas),
	%altura(P, Regra ),
	%pergunta_exatidao(E1, Regra),
	!. 
	
