#ifndef LEGENDA_CPP_
#define LEGENDA_CPP_

#include "legenda.h"

Legenda :: Legenda( QWidget *pai ) : QWidget( pai )
{
    ui = new Ui_Legenda;

    ui->setupUi( this );

    inicializar();
}


Legenda :: ~Legenda()
{
    delete ui;
}


void Legenda :: adicionar_conjunto( const QString &nome_conj )
{
    // Criando o layout que irá conter as curvas
    QVBoxLayout *novo_layout = new QVBoxLayout;

    novo_layout->setSpacing( 0 );
    novo_layout->setContentsMargins( 0, 0, 10, 0 );

    map_conj_layout[ nome_conj ] = novo_layout;

    QWidget *itens_legenda = new QWidget;

    itens_legenda->setLayout( novo_layout );

    ui->paginas->addItem( itens_legenda, nome_conj );
}


void Legenda :: adicionar_curva( const QString &nome_conj, QwtPlotCurve *curva )
{
    QVBoxLayout *layout = map_conj_layout[ nome_conj ];

    CaixaMarcacao *caixa_marcacao = new CaixaMarcacao( curva );

    connect( caixa_marcacao, SIGNAL( item_atualizado() ),
             this, SLOT( item_legenda_atualizado() ) );

    layout->addWidget( caixa_marcacao );
}


void Legenda :: adicionar_deteccao( const QString &nome_conj, Retangulo *ret )
{
}


void Legenda :: limpar()
{
}


void Legenda :: inicializar()
{
    ui->paginas->removeItem( 0 );
}


void Legenda :: item_legenda_atualizado() 
{
    emit legenda_atualizada();
}

#endif
