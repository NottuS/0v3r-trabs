install: dissertacao.ps

dissertacao.ps: dissertacao.dvi
	dvips -o dissertacao.ps dissertacao.dvi


clean: 
	rm -f *.aux
	rm -f *.bbl
	rm -f *.bak
	rm -f *.log
	rm -f *.blg 
	rm -f *.toc
	rm -f *.lot
	rm -f *.lof
	rm -f *.dvi
	rm -f *.idx 
	rm -f *.ilg 
	rm -f *.ind

dissertacao.dvi: dissertacao.tex capitulo1.tex capitulo2.tex dissertacao.bbl
	latex dissertacao
	latex dissertacao

dissertacao.bbl: bibliografia.bib
	latex dissertacao
	bibtex dissertacao

