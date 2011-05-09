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


void JanelaInterna :: configurar_curvas( const QString &nome_falha,
                                         const MatrizD &dados,
                                         const QHash< int, QString > &curvas,
                                         const double &periodo_amostragem )
{
    // Adicionando o conjunto das curvas
    try
    {
        ui->grafico->adicionar_conjunto( nome_falha );
    }
    catch( Excecao e )
    {
        // Se o conjunto já existir, não precisa exibir nenhuma mensagem de
        // erro. Basta atualizar os valores das curvas...
        ui->grafico->remover_curvas( nome_falha );
    }

    // Adicionando as curvas ao conjunto. Perceba que se o conjunto não existia,
    // as curvas serão criadas pela primeira vez e se o conjunto já existia, as
    // curvas foram removidas e serão criadas novamente
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
        ui->grafico->adicionar_curva( nome_falha, 
                                      nomes_curvas[c], 
                                      QPen( cor ) );

        // Inserindo os dados
        coluna = dados.get_column( curvas_escolhidas[c] );

        x = 0.0;
        y = 0.0;

        for ( int a = 0 ; a < coluna.get_size() ; a++ )
        {
            x += periodo_amostragem;
            y = coluna[a];

            ui->grafico->adicionar_xy( nome_falha, 
                                       nomes_curvas[c], 
                                       x, 
                                       y, 
                                       false );
        }

        coluna.set( 0 );
    }

    ui->grafico->atualizar();
}


void JanelaInterna :: configurar_deteccoes( const QString &nome_falha,
                                            const QHash< QString, 
                                                         MatrizI > &deteccoes )
{
    QList< QString > nomes_deteccoes = deteccoes.keys();
    QList< MatrizI > intervalos = deteccoes.values();

    MatrizI saida;

    QPair< double, double > inicio_fim;

    QColor cor;
    
    int num_cores = lista_cores.count();

    for ( int i = 0 ; i < intervalos.count() ; i++ )
    {
        // Configurando a cor da curva
        if ( i >= num_cores )
        {
            cor = QColor( rand() % 256, rand() % 256, rand() % 256, 255 );
        }
        else
        {
            cor = lista_cores[i];
        }

        // Transparencia de 50% para as deteccoes
        cor.setAlpha( 150 );

        saida = intervalos[i];

        for ( int a = 0 ; a < saida.get_rows_number() ; a++ )
        {
            inicio_fim.first = saida[a][0];
            inicio_fim.second = saida[a][1];
                
            ui->grafico->adicionar_deteccao( nome_falha, 
                                             nomes_deteccoes[i] );
        }
    }
}


void JanelaInterna :: exibir_curvas( const bool &b )
{
}


void JanelaInterna :: exibir_deteccoes( const bool &b )
{
}


void JanelaInterna :: limpar()
{
    ui->grafico->limpar();
    ui->legenda->limpar();
}


void JanelaInterna :: remover_curvas( const QString &nome_falha )
{
    try
    {
        ui->grafico->remover_curvas( nome_falha );
    }
    catch( Excecao e )
    {
        throw e;
    }
}


void JanelaInterna :: remover_deteccoes( const QString &nome_falha )
{
    try
    {
        ui->grafico->remover_deteccoes( nome_falha );
    }
    catch( Excecao e )
    {
        throw e;
    }
}


void JanelaInterna :: inicializar()
{
    // Associando a legenda ao grafico
    ui->grafico->configurar_legenda( ui->legenda );

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
