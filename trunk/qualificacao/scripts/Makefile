all: resultados

resultados: resultados.gp
	@echo Gerando os graficos...
	gnuplot resultados.gp
	mv *.eps ..

clean:
	@echo Removendo arquivos...
	-rm *.eps *.tex
