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

        if ( !legenda->isVisible() )
            legenda->setVisible( true );
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
                                    const QString &nome_detec,
                                    const QPen &linha,
                                    const QBrush &preenchimento )
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
        conjuntos[indice_conj].adicionar_deteccao( nome_detec, 
                                                   linha, 
                                                   preenchimento);
    }
    catch( Excecao e )
    {
        throw e;
    }

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        QPair< QPen, QBrush > estilos;

        legenda->adicionar_deteccao( nome_conj,
                                     nome_detec,
                              conjuntos[indice_conj].estilos( nome_detec ),
                              conjuntos[indice_conj].retangulos( nome_detec ) );
    }
}


void Grafico :: adicionar_intervalo_detec( const QString &nome_conj,
                                           const QString &nome_detec,
                                           const QPair< double, 
                                                        double > &inicio_fim,
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
        qreal y_inicial = escala->upperBound();
        qreal larg = inicio_fim.second - x_inicial;
        qreal alt = y_inicial - escala->lowerBound();

        QRectF ret( x_inicial, y_inicial, larg, alt );

        conjuntos[indice_conj].adicionar_intervalo_detec( nome_detec, ret );
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


void Grafico :: configurar_nomes_eixos( const QString &x, const QString &y )
{
    QwtText texto_eixo_x( x );
    QwtText texto_eixo_y( y );
    
    QFont fonte = this->font();

    fonte.setBold( true );

    texto_eixo_x.setFont( fonte );
    texto_eixo_y.setFont( fonte );

    this->setAxisTitle( QwtPlot::xBottom, texto_eixo_x );
    this->setAxisTitle( QwtPlot::yLeft, texto_eixo_y );
}


void Grafico :: imprimir()
{
    QPrinter impressora( QPrinter::HighResolution );

    QString nome_doc = this->title().text();

    if ( !nome_doc.isEmpty() )
    {
        nome_doc.replace( QRegExp( utf8( "\n" ) ), tr(" -- ") );
        impressora.setDocName( nome_doc );
    }

    impressora.setCreator( "Simddef" );
    impressora.setOrientation( QPrinter::Landscape );

    QPrintDialog dialog( &impressora );

    if ( dialog.exec() )
    {
        QwtPlotRenderer renderizador;

        if ( impressora.colorMode() == QPrinter::GrayScale )
        {
            renderizador.setDiscardFlag( QwtPlotRenderer::DiscardCanvasBackground );
            renderizador.setLayoutFlag( QwtPlotRenderer::FrameWithScales );
        }

        renderizador.renderTo( this, impressora );
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

    // Removendo os itens da legenda
    if ( legenda != NULL )
    {
        legenda->limpar();
        legenda->setVisible( false );
    }

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

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        legenda->remover_conjunto( nome_conj );

        if ( nomes_conjuntos.count() == 0 )
            legenda->setVisible( false );
    }

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


void Grafico :: salvar()
{
    QString nome_arq;

#ifndef QT_NO_PRINTER
    nome_arq = "simddef.pdf";
#else
    nome_arq = "simddef.png";
#endif

#ifndef QT_NO_FILEDIALOG
    const QList< QByteArray > formatos_imagem = 
                              QImageWriter::supportedImageFormats();

    QStringList filtro;
    
    filtro << "Portable Document Format (*.pdf)";
#ifndef QWT_NO_SVG
    filtro << "Scalable Vector Graphics (*.svg)";
#endif
    filtro << "PostScript (*.ps)";

    if ( formatos_imagem.size() > 0 )
    {
        QString filtro_img ( "Imagens (");

        for ( int i = 0; i < formatos_imagem.size(); i++ )
        {
            if ( i > 0 )
                filtro_img += " ";

            filtro_img += "*.";
            filtro_img += formatos_imagem[i];
        }

        filtro_img += ")";

        filtro << filtro_img;
    }

    nome_arq = QFileDialog::getSaveFileName( pai, 
                                             "Exportar arquivo como...", 
                                             nome_arq,
                                             filtro.join( ";;" ), 
                                             NULL, 
                                             QFileDialog::DontConfirmOverwrite );
#endif

    if ( !nome_arq.isEmpty() )
    {
        QwtPlotRenderer renderizador;

        // flags to make the document look like the widget
        renderizador.setDiscardFlag( QwtPlotRenderer::DiscardBackground, false );
        renderizador.setLayoutFlag( QwtPlotRenderer::KeepFrames, true );

        qreal larg = 500;
        qreal alt = ( 10 * larg ) / 16; // Formato 16:10

        renderizador.renderDocument( this, nome_arq, QSizeF( larg, alt ), 100 );
    }

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
    zoom->setRubberBandPen( QPen( Qt::blue, 1, Qt::DashLine ) );
    zoom->setTrackerPen( QPen( Qt::blue ) );
    zoom->setTrackerMode( QwtPicker::AlwaysOn );
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


void Grafico :: atualizar_deteccoes( const bool &replote )
{
    if ( nomes_conjuntos.count() > 0 )
    {
        QList< Retangulo * > *lista_rets;

        QStringList nomes_deteccoes;

        QwtScaleDiv *escala = this->axisScaleDiv( QwtPlot::yLeft );

        qreal y_inicial = escala->lowerBound();
        qreal altura = escala->upperBound() - y_inicial;

        for ( int c = 0 ; c < conjuntos.count() ; c++ )
        {
            nomes_deteccoes = conjuntos[c].deteccoes();

            for ( int d = 0 ; d < nomes_deteccoes.count() ; d++ )
            {
                lista_rets = conjuntos[c].retangulos( nomes_deteccoes[d] );

                for ( int r = 0 ; r < lista_rets->count() ; r++ )
                {
                    (*lista_rets)[r]->configurar_eixo_y( y_inicial, altura );
                }
            }
        }

        if ( replote )
            this->replot();
    }
}


void Grafico :: atualizar_escala( const bool &replote )
{
    this->setAxisAutoScale( QwtPlot::yLeft );
    this->setAxisAutoScale( QwtPlot::xBottom );

    if ( replote )
        this->replot();

    zoom->setZoomBase();
}


void Grafico :: habilitar_legenda( const bool &b )
{
    legenda->setVisible( b );
}


void Grafico :: habilitar_zoom( const bool &b )
{
    zoom->setEnabled( b );

    atualizar_escala( true );
}

#endif
