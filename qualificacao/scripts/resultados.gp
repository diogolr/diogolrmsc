# configuracao da saida ########################################################
#set term postscript eps enhanced color "Times" 22
set term epslatex color
set encoding utf8

# excluindo o titulo
unset label

# ajustando a margem inferior
set bmargin 4

# ajustando os tics dos eixos
set xtic auto border nomirror out
set ytic auto border nomirror out

# determinando o formato de exibicao
#set format y "%0.2f"
#set format x "%0.0f"

# configuracao das linhas ######################################################
set style line 1 linetype 1 linecolor rgb "#FF0000" linewidth 2 # Vermelho
set style line 2 linetype 1 linecolor rgb "#00FF00" linewidth 2 # Verde
set style line 3 linetype 1 linecolor rgb "#0000FF" linewidth 2 # Azul
set style line 4 linetype 1 linecolor rgb "#000000" linewidth 2 # Preto
set style line 5 linetype 2 linecolor rgb "#8B8B8B" linewidth 1 # Cinza (Grid)

set style function lines

set grid ls 5

# inicio dos plots #############################################################
set xlabel 'ads $\displaystyle\int_ a^b l_i di$'
set ylabel 'Este é o eixo $y$ e teste $\Delta$'

set xrange [0:1]
set style rect fc lt -1 fs solid 0.1 noborder
set obj rect from 0.5, graph 0 to 0.55, graph 1
set output "teste.tex"
plot sin(x)
unset obj

set xrange [-10:10]
set xlabel 'Teste final $\alpha$'
set ylabel 'Este é o eixo $y$'
set output "teste2.tex"
plot sin(x) title '$\alpha$ e $\beta$ como título' ls 3 with lines

#plot "../dados/acp/completo/ordem_2/validacao_1/O2N10R2V1A_DADOS.dat" \
#     using 1:2 title 'simulado' ls 1 with lines,\
#     "../dados/acp/completo/ordem_2/validacao_1/O2N10R2V1A_DADOS.dat" \
#     using 1:5 title 'estimado' ls 2 with lines
