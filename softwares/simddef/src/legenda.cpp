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


void Legenda :: adicionar_item( QwtPlotCurve *curva )
{
    // Layout horizontal
    QHBoxLayout *layout_horizontal = new QHBoxLayout;

    layouts.push_back( layout_horizontal );

    // Checkbox
    QCheckBox *item = new QCheckBox( this );

    mapeamento_cb_curva[ item ] = curva;

    item->setText( curva->title().text() );
    item->setChecked( true );
    item->setMinimumWidth( 135 );

    connect( item, SIGNAL( stateChanged( int ) ), 
             this, SLOT( mudanca_estado() ) );

    // Graphics view
    QGraphicsView *imagem = new QGraphicsView( this );
    imagem->setMaximumWidth( 25 );
    imagem->setMaximumHeight( 16 );

    QGraphicsScene *cena = new QGraphicsScene( imagem );
    imagem->setScene( cena );

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


void Legenda :: limpar()
{
    // TODO
}


void Legenda :: inicializar()
{
    grafico = NULL;
}


void Legenda :: mudanca_estado()
{
    QCheckBox *cb = (QCheckBox*)QObject::sender();

    QwtPlotCurve *curva = mapeamento_cb_curva[ cb ];

    switch( cb->checkState() )
    {
        case Qt::PartiallyChecked:
        case Qt::Checked:
            curva->setVisible( true );
            break;

        case Qt::Unchecked:
            curva->setVisible( false );
            break;
    }
            
    if ( grafico != NULL )
        grafico->replot();
}

#endif
