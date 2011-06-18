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
	pdftk principal.pdf update_info metadata output dissertacao.pdf
	rm -f principal.pdf
	evince dissertacao.pdf&

apres: apresentacao.tex
	latex -shell-escape apresentacao
	latex -shell-escape apresentacao
	dvipdf apresentacao
	mv apresentacao.pdf apres.pdf
	pdftk apres.pdf update_info metadata output apresentacao.pdf
	rm -f apres.pdf
	evince apresentacao.pdf&

clean: 
	-cd scripts && make clean
	-rm -f *.pdf *.aux *.dvi *.ps *.lo* *.toc *.bbl *.log *.blg 
	-rm -f *.gl* *.ilg *.out *.pyg *.nav *.snm
