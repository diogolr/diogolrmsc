#all: clean script apres principal
all: clean script principal

#apres: apresentacao/Makefile
#	cd apresentacao && make
	
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

clean: 
	-cd scripts && make clean
#	-cd apresentacao && make clean
	-rm -f *.pdf *.aux *.dvi *.ps *.lo* *.toc *.bbl *.log *.blg *.gl* *.ilg *.out
