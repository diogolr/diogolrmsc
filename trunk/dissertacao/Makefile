all: clean script principal

script: scripts/Makefile
	cd scripts && make

principal: principal.tex
	latex principal
	makeindex -s makeglo.ist -o principal.gls principal.glo
	bibtex principal 
	latex principal
	latex principal
	dvipdf principal
	evince principal.pdf&

apres: apresentacao.tex
	latex apresentacao
	latex apresentacao
	dvipdf apresentacao
	evince apresentacao.pdf&

clean: 
	-cd scripts && make clean
	-rm -f *.pdf *.aux *.dvi *.ps *.lo* *.toc *.bbl *.log *.blg *.gl* *.ilg *.out
	-rm -f *.nav *.snm
