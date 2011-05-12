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
    // Layout que conterá o layout das curvas e um espaçador vertical
    QVBoxLayout *layout = new QVBoxLayout;

    layout->setContentsMargins( 0, 0, 0, 0 );
    layout->setSpacing( 0 );

    // Criando o layout que irá conter as curvas
    QVBoxLayout *layout_curvas = new QVBoxLayout;

    layout_curvas->setSpacing( 0 );
    layout_curvas->setContentsMargins( 10, 0, 10, 0 );

    map_conj_layout[ nome_conj ] = layout_curvas;

    // Adicionando os elementos do layout principal
    layout->addLayout( layout_curvas );
    layout->addStretch( 1 );

    // Widget a ser adicionada (nova página do QToolBox)
    QWidget *itens_legenda = new QWidget;

    map_conj_widget[ nome_conj ] = itens_legenda;

    itens_legenda->setLayout( layout );

    ui->paginas->addItem( itens_legenda, nome_conj );

    ui->paginas->setCurrentWidget( itens_legenda );
}


void Legenda :: adicionar_curva( const QString &nome_conj, QwtPlotCurve *curva )
{
    QVBoxLayout *layout = map_conj_layout[ nome_conj ];

    CaixaMarcacao *caixa_marcacao = new CaixaMarcacao( curva );

    connect( caixa_marcacao, SIGNAL( item_atualizado() ),
             this, SLOT( item_legenda_atualizado() ) );

    layout->addWidget( caixa_marcacao );

    /*
    // Mapeando o nome do conjunto + curva para a caixa de marcação criada
    QPair< QString, QString > conj_curva;
    
    conj_curva.first = nome_conj;
    conj_curva.second = curva->title().text();

    map_conj_curva_caixa_marc[ conj_curva ] = caixa_marcacao;
    */
}


void Legenda :: adicionar_deteccao( const QString &nome_conj,
                                    const QString &nome_detec,
                                    const QPair< QPen, QBrush > &estilos,
                                    QList< Retangulo * > *rets )
{
    QVBoxLayout *layout = map_conj_layout[ nome_conj ];

    CaixaMarcacao *caixa_marcacao = new CaixaMarcacao( nome_detec, estilos, rets );

    connect( caixa_marcacao, SIGNAL( item_atualizado() ),
             this, SLOT( item_legenda_atualizado() ) );

    layout->addWidget( caixa_marcacao );

    /*
    // Mapeando o nome do conjunt + nome da detecção para a caixa de marcação
    // criada
    QPair< QString, QString > conj_detec;

    conj_detec.first = nome_conj;
    conj_detec.second = nome_detec;

    map_conj_detec_caixa_marc[ conj_detec ] = caixa_marcacao;
    */
}


void Legenda :: limpar()
{
}


void Legenda :: remover_conjunto( const QString &nome_conj )
{
    int indice = ui->paginas->indexOf( map_conj_widget[ nome_conj ] );

    ui->paginas->removeItem( indice );

    delete map_conj_layout[ nome_conj ];

    map_conj_layout.remove( nome_conj );
    map_conj_widget.remove( nome_conj );
}


/*
void Legenda :: remover_curva( const QString &nome_conj, 
                               const QString &nome_curva )
{
    QPair< QString, QString > conj_curva;
    
    conj_curva.first = nome_conj;
    conj_curva.second = nome_curva;

    delete map_conj_curva_caixa_marc[ conj_curva ];

    map_conj_curva_caixa_marc.remove( conj_curva );
}


void Legenda :: remover_deteccao( const QString &nome_conj, 
                                  const QString &nome_detec )
{
    QPair< QString, QString > conj_detec;

    conj_detec.first = nome_conj;
    conj_detec.second = nome_detec;

    delete map_conj_detec_caixa_marc[ conj_detec ];
    
    map_conj_detec_caixa_marc.remove( conj_detec );
}
*/


void Legenda :: inicializar()
{
    ui->paginas->removeItem( 0 );
}


void Legenda :: item_legenda_atualizado() 
{
    emit legenda_atualizada();
}

#endif
