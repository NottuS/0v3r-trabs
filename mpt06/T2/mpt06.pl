base_de_regras(
[
[ [idade, jovem], [especie, humana], [sexo, masculino], -> , 1.0, [chamado, menino] ],
[ [idade, jovem], [especie, humana], [sexo, feminino], -> , 1.0, [chamado, menina] ],
[ [idade, adulto], [especie, humana], [sexo, masculino], -> , 1.0, [chamado, homem] ],
[ [idade, adulto], [especie, humana], [sexo, feminino], -> , 1.0, [chamado, mulher] ],
[ [idade, jovem], [especie, cao], ->, 1.0, [chamado, filhote] ],
[ [idade, adulto], [especie, cao], [sexo, feminino], ->, 1.0, [chamado, cadela] ],
[ [idade, adulto], [especie, cao], ->, 0.5, [chamado, cao] ],
[ [pernas, 2], ->, 1.0, [especie, humana] ],
[ [pernas, 4], ->, -1.0, [especie, humana] ],
[ [pernas, 4], ->, 0.5, [especie, cao] ],
[ [altura, baixa], ->, 0.5, [especie, cao] ],
[ [altura, alta], ->, 0.5, [especie, humana] ]
]).

imprimeDescobrindo(Atributo):-
	write('>> Descobrindo sobre '),write(Atributo),write('.'),nl,fail.

imprimeTentando(Regra):-
	write('>> Tentando a regra '),write(Regra),write('.'),nl,fail.

imprimeAbandonando(Atributo):-
	write('>> Abandonando esta regra para '),write(Atributo),write('.'),nl,fail.

imprimeAtualizar(Atributo,V,E):-
	write('>> A exatidao de '),write(Atributo),
	write(' = '),write(V),
	write(' agora eh ' ),write(E),write('.'),nl,fail.

imprimeInserir(Atributo,V,E):-
	write('>> A exatidao de '),write(Atributo),
	write(' = '),write(V),
	write(' passou a ser ' ),write(E),write('.'),nl,fail.

imprimePorque(Regra):-
	write('Eu estou tentando usar a regra:'),nl,write('\t'),write(Regra),nl.

novaLinha(_):-
	retract(monitoramento),
	assertz(monitoramento),
	nl,!;
	true.

perguntaExatidao(Regra,Exatidao):-
	write('Qual eh o seu grau de exatidao (um numero entre -1 e 1) ? '),
	read(Exatidao),
	Exatidao \= porque,
	novaLinha(_),!;
	imprimePorque(Regra),
	perguntaExatidao(Regra,Exatidao).

perguntaValor(Atributo,Regra,Valor):-
	nl,write('Me informe sobre '),write(Atributo),write(' ? '),
	read(Valor),
	Valor \= porque,!;
	imprimePorque(Regra),
	perguntaValor(Atributo,Regra,Valor).

provavel([Valor,E],[_,Valor],E):-
	limiar_de_exatidao(L),
	E >= L.

verifica([X|L],C,Exatidao):-
	provavel(X,C,Exatidao),!;
	verifica(L,C,Exatidao).

fato(Atributo,[[Atributo|L]|_],Fato):-
	Fato = L.

fato(Atributo,[_|L],Fato):-
	fato(Atributo,L,Fato).

procuraRegra(Atributo):-
	base_de_regras(BR),
	buscaRegra(BR,Atributo),!;
	true.

novoFato(Atributo,V,E,Fato):-
	retract(monitoramento),
	assertz(monitoramento),
	imprimeInserir(Atributo,V,E),!;
	retract(base_de_fatos(L)),
	assertz(base_de_fatos([[Atributo, [V, E]]|L])),
	Fato = [[V, E]].

antecedente([Atributo,_],Regra,Fato):-
	retract(monitoramento),
	assertz(monitoramento),
	imprimeDescobrindo(Atributo),!;
	base_de_fatos(BF),
	fato(Atributo,BF,Fato),!;
	procuraRegra(Atributo),
	base_de_fatos(BF),
	fato(Atributo,BF,Fato),!;
	perguntaValor(Atributo,Regra,V),
	perguntaExatidao(Regra,E),
	novoFato(Atributo,V,E,Fato).

min(X,Y,Min):-
	Min is abs(X),
	abs(X) < abs(Y),!;
	Min is abs(Y).

calcula(Antiga, Nova, Resultante):- 
	Antiga > 0, 
	Nova > 0,
	Resultante is (Antiga + Nova - (Antiga * Nova)),!;
	Antiga < 0,
	Nova < 0, 
	Resultante is (Antiga + Nova + (Antiga * Nova)),!;
	min(Antiga, Nova, Min),
	Resultante is ((Antiga + Nova) / (1 - Min)).

atualizaValor([[Valor,Q]|L],Valor,Nova,Resultante,[[Valor, Resultante]|L]):-
	calcula(Q, Nova, Resultante).

atualizaValor([C|L],Valor,Nova,Resultante,[C|V]):-
	atualizaValor(L,Valor,Nova,Resultante,V).

valorAtualizado(Atributo,Valor,Resultante):-
	retract(monitoramento),
	assertz(monitoramento),
	imprimeInserir(Atributo,Valor,Resultante),!;
	true.

novoValor(Atributo,Valor,Nova,L,V):-
	retract(monitoramento),
	assertz(monitoramento),
	imprimeInserir(Atributo,Valor,Nova),!;
	V = [[Valor, Nova]|L].

atualizaFato([[Atributo|L]|X],[Exatidao,[Atributo, Valor]],Menor,[[Atributo|V]|X]):-
	Nova is Menor * Exatidao,
	atualizaValor(L,Valor,Nova,Resultante,V),
	valorAtualizado(Atributo,Valor,Resultante),!;
	Nova is Menor * Exatidao,
	novoValor(Atributo,Valor,Nova,L,V).

atualizaFato([C|X],L,Menor,[C|A]):-
	atualizaFato(X,L,Menor,A).

insereFato([Exatidao,[Atributo, Valor]],Menor):-
	retract(monitoramento),
	assertz(monitoramento),
	E is Menor * Exatidao,
	imprimeInserir(Atributo,Valor,E),!;
	Nova is Menor * Exatidao,
	retract(base_de_fatos(L)),
	assertz(base_de_fatos([[Atributo, [Valor, Nova]]|L])).

descobrir([->|L],_,Menor):-
	base_de_fatos(BF),
	atualizaFato(BF,L,Menor,Novos),
	retractall(base_de_fatos(_)),
	assertz(base_de_fatos(Novos)),!;
	insereFato(L,Menor).

descobrir([C|L],Regra,Menor):-
	antecedente(C,Regra,Fato),
	verifica(Fato,C,E),
	min(E,Menor,Min),
	descobrir(L,Regra,Min).

consequente([_,[Atributo,_]|_],Atributo).

buscaConsequente([->|L],Atributo):-
	consequente(L,Atributo).

buscaConsequente([_|L],Atributo):-
	buscaConsequente(L,Atributo).

verificaRegra(R,Menor,Atributo):-
	retract(monitoramento),
	assertz(monitoramento),
	imprimeTentando(R),!;
	descobrir(R,R,Menor),!;
	retract(monitoramento),
	assertz(monitoramento),
	imprimeAbandonando(Atributo).

buscaRegra([R|L],Atributo):-
	buscaConsequente(R,Atributo),
	Menor is -1,
	verificaRegra(R,Menor,Atributo),fail,!;
	buscaRegra(L,Atributo).

hipoteses(Atributo):-
	nl,write('As hipoteses sao as seguintes: '),
	base_de_fatos(BF),
	fato(Atributo,BF,Fato),
	write(Fato),!;
	write('[]').

testaMonitoramento(_):-
	retract(monitoramento),!.

diagnostico(Atributo):-
	testaMonitoramento(_),
	retractall(monitoramento),
	assertz(monitoramento),
	nl,imprimeDescobrindo(Atributo),!;
	retractall(base_de_fatos(_)),
	assertz(base_de_fatos([])),
	assertz(limiar_de_exatidao(0.2)),
	base_de_regras(BR),
	buscaRegra(BR,Atributo),
	hipoteses(Atributo),!;
	hipoteses(Atributo).
