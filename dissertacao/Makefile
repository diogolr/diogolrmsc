all: clean script principal

script: scripts/Makefile
	cd scripts && make

principal: principal.tex
	latex -shell-escape principal
	makeindex -s makeglo.ist -o principal.gls principal.glo
	bibtex principal 
	latex -shell-escape principal
	latex -shell-escape principal
	dvipdf principal
	evince principal.pdf&

apres: apresentacao.tex
	latex apresentacao
	latex apresentacao
	dvipdf apresentacao
	evince apresentacao.pdf&

clean: 
	-cd scripts && make clean
	-rm -f *.pdf *.aux *.dvi *.ps *.lo* *.toc *.bbl *.log *.blg 
	-rm -f *.gl* *.ilg *.out *.pyg *.nav *.snm
