#ifndef GRAFICO_CPP_
#define GRAFICO_CPP_

#include "grafico.h"

Grafico :: Grafico( QWidget *p ) : QwtPlot( p ), pai( p )
{
    configurar();
}


Grafico :: ~Grafico()
{
    if ( zoom != NULL )
        delete zoom;
}


void Grafico :: adicionar_conjunto( const QString &nome_conj )
{
    if ( nomes_conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Já existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    nomes_conjuntos << nome_conj;

    ConjuntoItens novo_conj( this, nome_conj );        

    conjuntos << novo_conj;

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        legenda->adicionar_conjunto( nome_conj );
    }
}


void Grafico :: adicionar_curva( const QString &nome_conj, 
                                 const QString &nome_curva,
                                 const QPen &linha,
                                 const QwtPlotCurve::CurveStyle &estilo )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    try
    {
        conjuntos[indice_conj].adicionar_curva( nome_curva, linha, estilo );
    }
    catch( Excecao e )
    {
        throw e;
    }

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        legenda->adicionar_curva( nome_conj, 
                                  conjuntos[indice_conj].curva( nome_curva ) );
    }
}


void Grafico :: adicionar_deteccao( const QString &nome_conj,
                                    const QString &nome_detec )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    try
    {
        conjuntos[indice_conj].adicionar_deteccao( nome_detec );
    }
    catch( Excecao e )
    {
        throw e;
    }

    /*
    // Atualizando a legenda
    if ( legenda != NULL )
    {
        // TODO
    }
    */
}


void Grafico :: adicionar_intervalo_detec( const QString &nome_conj,
                                           const QString &nome_detec,
                                           const QPair< double, 
                                                        double > &inicio_fim,
                                           const QPen &linha,
                                           const QBrush &preenchimento,
                                           const bool &replote )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    try
    {
        QwtScaleDiv *escala = this->axisScaleDiv( QwtPlot::yLeft );

        qreal x_inicial = inicio_fim.first;
        qreal y_inicial = escala->lowerBound();
        qreal larg = inicio_fim.second - x_inicial;
        qreal alt = escala->upperBound() - y_inicial;

        QRectF ret( x_inicial, y_inicial, larg, alt );

        conjuntos[indice_conj].adicionar_intervalo_detec( nome_detec, 
                                                          ret, 
                                                          linha, 
                                                          preenchimento );
    }
    catch( Excecao e )
    {
        throw e;
    }

    if ( replote )
        this->replot();
}


void Grafico :: adicionar_xy( const QString &nome_conj, 
                              const QString &nome_curva,
                              const double &x,
                              const double &y,
                              const bool &replote )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    try
    {
        conjuntos[indice_conj].adicionar_xy( nome_curva, x, y );
    }
    catch( Excecao e )
    {
        throw e;
    }

    if ( replote )
        this->replot();
}


void Grafico :: configurar_legenda( Legenda *l )
{
    if ( l != NULL )
    {
        legenda = l;

        connect( legenda, SIGNAL( legenda_atualizada() ),
                 this, SLOT( atualizar() ) );
    }
}


void Grafico :: limpar( const bool &replote )
{
    // Removendo todos os conjuntos
    while( !conjuntos.isEmpty() )
    {
        conjuntos.takeFirst().limpar();
    }

    nomes_conjuntos.clear();

    if ( replote )
        this->replot();
}


void Grafico :: remover_conjunto( const QString &nome_conj, 
                                  const bool &replote )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }
    
    try
    {
        conjuntos[indice_conj].limpar();
        conjuntos.removeAt( indice_conj );

        nomes_conjuntos.removeAt( indice_conj );
    }
    catch( Excecao e )
    {
        throw e;
    }

    /*
    // Atualizando a legenda
    if ( legenda != NULL )
    {
        // TODO
    }
    */

    if ( replote )
        this->replot();
}


void Grafico :: remover_curva( const QString &nome_conj, 
                               const QString &nome_curva,
                               const bool &replote )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }
    
    try
    {
        conjuntos[indice_conj].remover_curva( nome_curva );
    }
    catch( Excecao e )
    {
        throw e;
    }

    if ( replote )
        this->replot();
}


void Grafico :: remover_curvas( const QString &nome_conj, 
                                const bool &replote )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }
    
    try
    {
        conjuntos[indice_conj].remover_curvas();
    }
    catch( Excecao e )
    {
        throw e;
    }

    if ( replote )
        this->replot();
}


void Grafico :: remover_deteccao( const QString &nome_conj, 
                                  const QString &nome_detec,
                                  const bool &replote )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }
    
    try
    {
        conjuntos[indice_conj].remover_deteccao( nome_detec );
    }
    catch( Excecao e )
    {
        throw e;
    }

    if ( replote )
        this->replot();
}


void Grafico :: remover_deteccoes( const QString &nome_conj, 
                                   const bool &replote )
{
    int indice_conj = nomes_conjuntos.indexOf( nome_conj );

    if ( indice_conj == -1 )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }
    
    try
    {
        conjuntos[indice_conj].remover_deteccoes();
    }
    catch( Excecao e )
    {
        throw e;
    }

    if ( replote )
        this->replot();
}


void Grafico :: configurar()
{
    // Modificando as fontes dos eixos
    for ( int i = 0 ; i < QwtPlot::axisCnt ; i++ )
    {
        setAxisFont( i, pai->font() );
    }

    // Ajustando o canvas
    canvas()->setFrameStyle( QFrame::Box | QFrame::Plain );
    canvas()->setLineWidth( 1 );
    canvas()->setBorderRadius( 15 );

    plotLayout()->setAlignCanvasToScales( true );

    // Gradiente do canvas
    QPalette pal = canvas()->palette();

#if QT_VERSION >= 0x040400
    QLinearGradient gradiente( 0.0, 0.0, 1.0, 0.0 );

    gradiente.setCoordinateMode( QGradient::StretchToDeviceMode );
    gradiente.setColorAt( 0.0, QColor( 235, 235, 235 ) );
    gradiente.setColorAt( 1.0, QColor( 179, 179, 179 ) );

    pal.setBrush( QPalette::Window, QBrush( gradiente ) );
#else
    pal.setBrush( QPalette::Window, QBrush( color ) );
#endif

    canvas()->setPalette( pal );

    // Grid
    QwtPlotGrid *grid = new QwtPlotGrid();

    grid->setPen( QPen( Qt::darkGray, 0.0, Qt::DotLine ) );
    grid->enableX( true );
    grid->enableXMin( true );
    grid->enableY( true );
    grid->enableYMin( false );
    grid->attach( this );

    // Zoom
    zoom = new QwtPlotZoomer( canvas() );

    zoom->setRubberBand( QwtPicker::RectRubberBand );

    zoom->setRubberBandPen( QPen( Qt::white, 0.5, Qt::SolidLine ) );

    zoom->setTrackerPen( QPen( Qt::white ) );

    zoom->setTrackerMode( QwtPicker::ActiveOnly );

    zoom->setEnabled( true );
    
    // The backing store is important, when working with widget overlays ( f.e
    // rubberbands for zooming ). 
    canvas()->setPaintAttribute( QwtPlotCanvas::BackingStore, true );

#if defined(Q_WS_X11)
    // Even if not recommended by TrollTech, Qt::WA_PaintOutsidePaintEvent
    // works on X11. This has a nice effect on the performance.
    canvas()->setAttribute( Qt::WA_PaintOutsidePaintEvent, true );
#endif

}


void Grafico :: inicializar()
{
    legenda = NULL;
    zoom = NULL;
}


void Grafico :: atualizar()
{
    this->replot();
}


void Grafico :: habilitar_legenda( const bool &b )
{
    legenda->setVisible( b );
}


void Grafico :: habilitar_zoom( const bool &b )
{
    zoom->setEnabled( b );

    if ( b )
    {
        zoom->setZoomBase();
    }
    else
    {
        this->setAxisAutoScale( QwtPlot::yLeft );
        this->setAxisAutoScale( QwtPlot::xBottom );
    }
}

#endif
