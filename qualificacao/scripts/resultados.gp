# configuracao da saida ########################################################
#set term postscript eps enhanced color "Times" 22
set term epslatex color
set encoding utf8

# excluindo o titulo
unset label

# ajustando as margens
#set bmargin 4

# ajustando os tics dos eixos
set xtic auto border nomirror out
set ytic auto border nomirror out
set mxtics

# determinando o formato de exibicao
#set format y "%0.2f"
#set format x "%0.0f"

# configuracao das linhas ######################################################
set style line 1  linetype 2 linecolor rgb "#A00000" linewidth 2 # Verm. escuro
set style line 2  linetype 1 linecolor rgb "#FF0000" linewidth 3 # Vermelho
set style line 3  linetype 2 linecolor rgb "#008000" linewidth 2 # Verde escuro
set style line 4  linetype 1 linecolor rgb "#00FF00" linewidth 2 # Verde
set style line 5  linetype 2 linecolor rgb "#000080" linewidth 2 # Azul escuro
set style line 6  linetype 1 linecolor rgb "#0088AA" linewidth 2 # Azul médio
set style line 7  linetype 1 linecolor rgb "#0000FF" linewidth 3 # Azul
set style line 8  linetype 1 linecolor rgb "#5500D4" linewidth 2 # Roxo
set style line 9  linetype 1 linecolor rgb "#008080" linewidth 2 # Ciano escuro
set style line 10 linetype 1 linecolor rgb "#00FFCC" linewidth 2 # Ciano
set style line 11 linetype 1 linecolor rgb "#FF0066" linewidth 2 # Magenta esc.
set style line 12 linetype 1 linecolor rgb "#AA00AA" linewidth 2 # Magenta
set style line 13 linetype 1 linecolor rgb "#FF6600" linewidth 2 # Laranja
set style line 14 linetype 1 linecolor rgb "#000000" linewidth 2 # Preto
set style line 15 linetype 2 linecolor rgb "#ADADAD" linewidth 1 # Cinza (Grid)

set style function lines

# configurando o grid
# set grid ls 15

# inicio dos plots #############################################################
set xlabel 'Tempo [s]'
set ylabel 'Nível [cm]'

set xrange [0:105]

# Legenda
set key inside left top vertical Left reverse enhanced autotitles nobox
set key noinvert samplen 1 spacing 1 width 0 height 0

# FSeDG ........................................................................
set output "fsedg_t1.tex"
load "../../dados/qualificacao/FSeDG_t1.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:2 title 'Ref. $T_1$' ls 1 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:4 title 'Saída $T_1$' ls 2 with lines

unset obj

set output "fsedg_t2.tex"
load "../../dados/qualificacao/FSeDG_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines

unset obj

# FSeDO ........................................................................
set output "fsedo.tex"
load "../../dados/qualificacao/FSeDO_t1.gp"
load "../../dados/qualificacao/FSeDO_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:2 title 'Ref. $T_1$' ls 1 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:4 title 'Saída $T_1$' ls 2 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines
unset obj

# FSeSR ........................................................................
# FSeQ  ........................................................................

# FADG .........................................................................
# FADO .........................................................................
# FASR .........................................................................
# FAVK .........................................................................
set output "favk_t1.tex"
load "../../dados/qualificacao/FAVK_t1.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FAVK/niveis_qualif.dat" \
     using 1:2 title 'Ref. $T_1$' ls 1 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FAVK/niveis_qualif.dat" \
     using 1:4 title 'Saída $T_1$' ls 2 with lines

unset obj

set output "favk_t2.tex"
load "../../dados/qualificacao/FAVK_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FAVK/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FAVK/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines

unset obj
# FAQ  .........................................................................

# FSiVzT   .....................................................................
# FSiVrOS  .....................................................................
# FSiVrGMP .....................................................................
# FSiEOS .......................................................................

# Outros .......................................................................
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
