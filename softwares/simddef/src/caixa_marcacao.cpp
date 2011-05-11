#ifndef CAIXA_MARCACAO_CPP_
#define CAIXA_MARCACAO_CPP_

#include "caixa_marcacao.h"

CaixaMarcacao :: CaixaMarcacao( QwtPlotCurve *c )
{
    ui = new Ui_CaixaMarcacao;

    ui->setupUi( this );

    inicializa_curva( c );
}

CaixaMarcacao :: CaixaMarcacao( const QString &nome_detec, 
                                const QList< Retangulo * > &rets )
{
    ui = new Ui_CaixaMarcacao;

    ui->setupUi( this );

    inicializa_deteccao( nome_detec, rets );
}


CaixaMarcacao :: ~CaixaMarcacao()
{
    delete ui;
}


void CaixaMarcacao :: inicializa_curva( QwtPlotCurve *c )
{
    // Inicializando a curva
    curva = c;

    // Configurando o label a ser exibido
    ui->label->setText( utf8( curva->title().text() ) );

    // Configurando o tipo da caixa de marcacao
    tipo_item = CaixaMarcacao::Curva;

    // Adicionando a imagem
    QGraphicsScene *cena = new QGraphicsScene( ui->imagem );

    ui->imagem->setScene( cena );

    cena->addLine( 0.0, 0.0, 25.0, 0.0, curva->pen() );
}


void CaixaMarcacao :: inicializa_deteccao( const QString &nome_detec,
                                           const QList< Retangulo * > &rets )
{
    // Inicializando a lista de retangulos
    retangulos = rets;

    // Configurando o label a ser exibido
    ui->label->setText( utf8( nome_detec ) );
    
    // Configurando o tipo da caixa de marcacao
    tipo_item = CaixaMarcacao::Deteccao;

    // Adicionando a imagem
    QGraphicsScene *cena = new QGraphicsScene( ui->imagem );

    ui->imagem->setScene( cena );

    cena->addRect( QRectF( 0.0, 0.0, 25.0, 5.0 ), 
                   retangulos[0]->linha(),
                   retangulos[0]->preenchimento() );
}


void CaixaMarcacao :: on_cb_stateChanged( int estado )
{
    bool marcado = false;

    switch( estado )
    {
        case Qt::PartiallyChecked:
        case Qt::Checked:
            marcado = true;
            break;

        case Qt::Unchecked:
            break;
    }

    switch( tipo_item )
    {
        case CaixaMarcacao::Curva:
            curva->setVisible( marcado );
            break;

        case CaixaMarcacao::Deteccao:
            for ( int i = 0 ; i < retangulos.count() ; i++ )
            {
                retangulos[i]->setVisible( marcado );
            }
            break;
    }

    emit item_atualizado();
}

#endif
