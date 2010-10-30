all: resultados

resultados: resultados.gp
	@echo Gerando os graficos...
	gnuplot resultados.gp
	mv *.eps *.tex ../imgs/resultados/eps

gerar_eps_inkscape:
	@echo Gerando as imagens em eps
	@bash ./gerar_eps.sh

clean:
	@echo Removendo arquivos...
	-rm ../imgs/resultados/eps/f*.eps ../imgs/resultados/eps/f*.tex
