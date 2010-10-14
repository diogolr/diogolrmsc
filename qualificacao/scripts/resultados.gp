# configuracao da saida ########################################################
#set term postscript eps enhanced color "Times" 22
set term epslatex color
set encoding utf8

# excluindo o titulo
unset label

# ajustando as margens
# set bmargin 4
# set bmargin 0

# ajustando os tics dos eixos
set xtic auto border nomirror out
set ytic auto border nomirror out
set mxtics

# determinando o formato de exibicao
#set format y "%0.2f"
#set format x "%0.0f"

# configuracao das linhas ######################################################
# set styke line <id> linetype <?> linecolor <?> linewidth
set style line 1  lt 2 lc rgb "#A00000" lw 2 # Verm. escuro
set style line 2  lt 1 lc rgb "#FF0000" lw 3 # Vermelho
set style line 3  lt 2 lc rgb "#008000" lw 2 # Verde escuro
set style line 4  lt 1 lc rgb "#00FF00" lw 2 # Verde
set style line 5  lt 2 lc rgb "#000080" lw 2 # Azul escuro
set style line 6  lt 1 lc rgb "#0088AA" lw 2 # Azul médio
set style line 7  lt 1 lc rgb "#0000FF" lw 3 # Azul
set style line 8  lt 1 lc rgb "#5500D4" lw 2 # Roxo
set style line 9  lt 1 lc rgb "#008080" lw 2 # Ciano escuro
set style line 10 lt 1 lc rgb "#00FFCC" lw 2 # Ciano
set style line 11 lt 1 lc rgb "#FF0066" lw 2 # Magenta esc.
set style line 12 lt 1 lc rgb "#AA00AA" lw 2 # Magenta
set style line 13 lt 1 lc rgb "#FF6600" lw 2 # Laranja
set style line 14 lt 1 lc rgb "#000000" lw 2 # Preto
set style line 15 lt 2 lc rgb "#ADADAD" lw 1 # Cinza (Grid)
set style line 16 lt 1 lc rgb "#AA0000" lw 2 # Vermelho (residuo)
set style line 17 lt 1 lc rgb "#00AA00" lw 2 # Azul (residuo)
set style function lines

# configurando o grid
set grid ls 15

# inicio dos plots #############################################################
set xlabel 'Tempo [s]'
set ylabel 'Nível [cm]' offset 2,0

set xrange [0:105]
set xtics 0,15,105

# Legenda
#set key inside left top vertical Left reverse enhanced autotitles nobox
set key noinvert samplen 1 spacing 1 width 0 height 0 autotitles nobox enhanced
set key bottom

# FSeDG ........................................................................
set output "fsedg.tex"
set multiplot layout 2,1

unset xlabel
set format x ""
set bmargin 0
set tmargin 0

set ytics 5,5
set size 1,0.5
load "../../dados/qualificacao/FSeDG_t1.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:2 title 'Ref. $T_1$' ls 1 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:4 title 'Saída $T_1$' ls 2 with lines
unset obj

set ytics 0,5
set size 1,0.5
set format x "%g"
set xlabel 'Tempo [s]'
load "../../dados/qualificacao/FSeDG_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines
unset obj
unset multiplot

# FSeDO ........................................................................
set output "fsedo.tex"
set multiplot layout 2,1

unset xlabel
set format x ""
set bmargin 0
set tmargin 0

set ytics 5,5
set size 1,0.5
load "../../dados/qualificacao/FSeDO_t1.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:2 title 'Ref. $T_1$' ls 1 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:4 title 'Saída $T_1$' ls 2 with lines
unset obj

set ytics 0,5
set size 1,0.5
set format x "%g"
set xlabel 'Tempo [s]'
load "../../dados/qualificacao/FSeDO_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDO/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines
unset obj
unset multiplot

# FSeSR ........................................................................
set output "fsesr.tex"
set multiplot layout 2,1

unset xlabel
set format x ""
set bmargin 0
set tmargin 0

set ytics 5,5
set size 1,0.5
load "../../dados/qualificacao/FSeSR_t1.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeSR/niveis_qualif.dat" \
     using 1:2 title 'Ref. $T_1$' ls 1 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeSR/niveis_qualif.dat" \
     using 1:4 title 'Saída $T_1$' ls 2 with lines
unset obj

set ytics 0,5
set size 1,0.5
set format x "%g"
set xlabel 'Tempo [s]'
load "../../dados/qualificacao/FSeSR_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeSR/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeSR/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines
unset obj
unset multiplot
# FSeQ  ........................................................................
set output "fseq.tex"
set multiplot layout 2,1

unset xlabel
set format x ""
set bmargin 0
set tmargin 0

set ytics 5,5
set size 1,0.5
load "../../dados/qualificacao/FSeQ_t1.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeQ/niveis_qualif.dat" \
     using 1:2 title 'Ref. $T_1$' ls 1 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeQ/niveis_qualif.dat" \
     using 1:4 title 'Saída $T_1$' ls 2 with lines
unset obj

set ytics 0,10
set size 1,0.5
set format x "%g"
set xlabel 'Tempo [s]'
load "../../dados/qualificacao/FSeQ_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeQ/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeQ/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines
unset obj
unset multiplot

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

set output "favk_t1_resid_sc.tex"
load "../../dados/qualificacao/FAVK_t1.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/erro_sc_qualif.dat" \
     using 1:4 title 'Sin. Cont. $T_1$' ls 2 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/erro_sc_qualif.dat" \
     using 1:2 title 'Erro $T_1$' ls 3 with lines,\
     "../../dados/qualificacao/FAVK_residuos.dat" \
     using 1:2 title 'Resíduo $T_1$' ls 4 with lines
unset obj

set output "favk_t2.tex"
load "../../dados/qualificacao/FAVK_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FAVK/niveis_qualif.dat" \
     using 1:3 title 'Ref. $T_2$' ls 5 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FAVK/niveis_qualif.dat" \
     using 1:5 title 'Saída $T_2$' ls 7 with lines
unset obj

set output "favk_t2_resid_sc.tex"
load "../../dados/qualificacao/FAVK_t2.gp"
plot "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/erro_sc_qualif.dat" \
     using 1:5 title 'Sin. Cont. $T_2$' ls 2 with lines,\
     "../../softwares/simulacao_tanques_cpp/saidas/FSeDG/erro_sc_qualif.dat" \
     using 1:3 title 'Erro $T_2$' ls 3 with lines,\
     "../../dados/qualificacao/FAVK_residuos.dat" \
     using 1:3 title 'Resíduo $T_2$' ls 4 with lines
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
