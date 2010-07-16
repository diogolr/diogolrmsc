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

    for ( int i = 0 ; i < layouts.size() ; i++ )
    {
        delete layouts[i];
        delete imagens[i];
    }

    layouts.clear();
    imagens.clear();
}


void Legenda :: adicionar_item( QwtPlotCurve *curva )
{
    // TODO verificar se ja n existe um item com o mesmo nome para que o
    // mapeamento seja feito corretamente
    
    // Curva
    curvas.push_back( curva );

    // Layout horizontal
    QHBoxLayout *layout_horizontal = new QHBoxLayout;

    layouts.push_back( layout_horizontal );

    // Checkbox
    QCheckBox *item = new QCheckBox( this );

    item->setObjectName( curva->title().text() );
    item->setText( curva->title().text() );
    item->setChecked( true );
    item->setMinimumWidth( 135 );

    nomes_itens.push_back( item->objectName() );

    connect( item, SIGNAL( stateChanged( int ) ), 
             this, SLOT( mudanca_estado() ) );

    // Graphics view
    QGraphicsView *imagem = new QGraphicsView( this );
    imagem->setMaximumWidth( 25 );
    imagem->setMaximumHeight( 16 );

    QGraphicsScene *cena = new QGraphicsScene( imagem );
    imagem->setScene( cena );

    imagens.push_back( imagem );

    cena->addLine( 0.0, 0.0, 25.0, 0.0, curva->pen() );

    // Adicionando aos layouts
    layout_horizontal->addWidget( item );
    layout_horizontal->addWidget( imagem );
    ui->layout_vertical->addLayout( layout_horizontal );
}


void Legenda :: associar_grafico( QwtPlot *g )
{
    grafico = g;
}


void Legenda :: inicializar()
{
    grafico = NULL;
}

#include <QDebug>
void Legenda :: mudanca_estado()
{
    QCheckBox *cb = (QCheckBox*)QObject::sender();

    int i = nomes_itens.indexOf( cb->objectName() );

    switch( cb->checkState() )
    {
        case Qt::PartiallyChecked:
        case Qt::Checked:
            curvas[i] ->setVisible( true );
            break;

        case Qt::Unchecked:
            curvas[i] ->setVisible( false );
            break;
    }
            
    if ( grafico != NULL )
        grafico->replot();
}

#endif
