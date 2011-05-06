#ifndef JANELA_INTERNA_CPP_
#define JANELA_INTERNA_CPP_

#include "janela_interna.h"

JanelaInterna :: JanelaInterna( QWidget *pai ) : QWidget( pai )
{
    ui = new Ui_JanelaInterna;

    ui->setupUi( this );

    inicializar();
}

JanelaInterna :: ~JanelaInterna()
{
    delete ui;
}


void JanelaInterna :: configurar_curvas( const MatrizD &dados,
                                         const QHash< int, QString > &curvas,
                                         const double &periodo_amostragem )
{
    QList< int > curvas_escolhidas = curvas.keys();
    QList< QString > nomes_curvas = curvas.values();

    int num_cores = lista_cores.count();
    int num_curvas = curvas_escolhidas.count();

    QColor cor;

    VetorD coluna;

    double x, y;

    for ( int c = 0 ; c < num_curvas ; c++ )
    {
        // Configurando a cor da curva
        if ( c >= num_cores )
        {
            cor = QColor( rand() % 256, rand() % 256, rand() % 256, 255 );
        }
        else
        {
            cor = lista_cores[c];
        }

        // Adicionando a curva ao grafico e a legenda
        ui->grafico->adicionar_curva( QPen( cor ), nomes_curvas[c] );
        ui->legenda->adicionar_item( ui->grafico->curva( c ) );

        // Inserindo os dados
        coluna = dados.get_column( curvas_escolhidas[c] );

        x = 0.0;
        y = 0.0;

        for ( int a = 0 ; a < coluna.get_size() ; a++ )
        {
            x += periodo_amostragem;
            y = coluna[a];

            ui->grafico->adicionar_xy( c, x, y, false );
        }

        coluna.set( 0 );
    }

    ui->grafico->replot();
}


void JanelaInterna :: configurar_deteccoes( const QHash< QString, 
                                                         MatrizI > &deteccoes )
{
    nomes_deteccoes = deteccoes.keys();
    intervalos = deteccoes.values();
}


void JanelaInterna :: exibir_curvas()
{
}


void JanelaInterna :: exibir_deteccoes()
{
}


void JanelaInterna :: limpar()
{
    // Limpando os intervalos e os nomes das deteccoes de falhas
    intervalos.clear();
    nomes_deteccoes.clear();

    ui->grafico->limpar();
    ui->legenda->limpar();
}


void JanelaInterna :: inicializar()
{
    lista_cores << Qt::red
                << Qt::blue
                << Qt::green
                << Qt::magenta
                << Qt::cyan
                << Qt::black
                << Qt::darkGreen
                << Qt::darkCyan
                << Qt::darkYellow
                << Qt::darkGray;
}

#endif
