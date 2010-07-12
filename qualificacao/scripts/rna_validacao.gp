# configuracao da saida ########################################################
set terminal postscript enhanced color "Helvetica" 22
set encoding iso_8859_15

# excluindo o titulo
unset label

# ajustando a margem inferior
set bmargin 4

# ajustando os tics dos eixos
set xtic auto border nomirror out
set ytic auto border nomirror out

# determinando o formato de exibicao
set format y "%0.2f"
set format x "%0.0f"

# configuracao das linhas ######################################################
set style line 1 linetype 1 linecolor rgb "#FF0000" linewidth 2
set style line 2 linetype 1 linecolor rgb "#0000FF" linewidth 2

set style function lines

# inicio dos plots #############################################################
set xlabel '{amostras}'
set ylabel '{fração molar (%)}

# ordem 2 ======================================================================
# validacao 1 ------------------------------------------------------------------
set output "O2V1_etano.eps"
plot "../dados/acp/completo/ordem_2/validacao_1/O2N10R2V1A_DADOS.dat" \
     using 1:2 title 'simulado' ls 1 with lines,\
     "../dados/acp/completo/ordem_2/validacao_1/O2N10R2V1A_DADOS.dat" \
     using 1:5 title 'estimado' ls 2 with lines
