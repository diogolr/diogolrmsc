all: resultados

resultados: resultados.gp
	@echo Gerando os graficos...
	gnuplot resultados.gp

gerar_eps_inkscape:
	@echo Gerando as imagens em eps
	@bash ./gerar_eps.sh

clean:
	@echo Removendo arquivos...
	-rm *.eps *.tex
