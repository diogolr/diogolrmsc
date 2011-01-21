all: clean cobem

cobem: cobem2011.tex
	latex cobem2011
	bibtex cobem2011
	latex cobem2011
	latex cobem2011
	dvipdf cobem2011.dvi
	evince cobem2011.pdf&

clean:
	-rm -f *.aux *.blg *.bbl *.pdf *.dvi *.log
