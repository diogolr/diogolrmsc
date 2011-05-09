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


double Grafico :: min_x()
{
    double min = DBL_MIN;
    
    QList< QList< QwtPlotCurve * > * > listas_curvas = map_conj_curvas.values();

    int num_curvas = 0;

    for ( int i = 0 ; i < listas_curvas.count() ; i++ )
    {
        num_curvas = listas_curvas[i]->count();

        for ( int j = 0 ; j < num_curvas ; j++ )
        {
            if ( (*listas_curvas[i])[j]->minXValue() < min )
                min = (*listas_curvas[i])[j]->minXValue();
        }
    }

    return min;
}


double Grafico :: min_y()
{
    double min = DBL_MIN;

    QList< QList< QwtPlotCurve * > * > listas_curvas = map_conj_curvas.values();

    int num_curvas = 0;

    for ( int i = 0 ; i < listas_curvas.count() ; i++ )
    {
        num_curvas = listas_curvas[i]->count();

        for ( int j = 0 ; j < num_curvas ; j++ )
        {
            if ( (*listas_curvas[i])[j]->minYValue() < min )
                min = (*listas_curvas[i])[j]->minYValue();
        }
    }

    return min;
}


double Grafico :: max_x()
{
    double max = DBL_MAX;

    QList< QList< QwtPlotCurve * > * > listas_curvas = map_conj_curvas.values();

    int num_curvas = 0;

    for ( int i = 0 ; i < listas_curvas.count() ; i++ )
    {
        num_curvas = listas_curvas[i]->count();

        for ( int j = 0 ; j < num_curvas ; j++ )
        {
            if ( (*listas_curvas[i])[j]->maxXValue() > max )
                max = (*listas_curvas[i])[j]->maxXValue();
        }
    }

    return max;
}


double Grafico :: max_y()
{
    double max = DBL_MAX;

    QList< QList< QwtPlotCurve * > * > listas_curvas = map_conj_curvas.values();

    int num_curvas = 0;

    for ( int i = 0 ; i < listas_curvas.count() ; i++ )
    {
        num_curvas = listas_curvas[i]->count();

        for ( int j = 0 ; j < num_curvas ; j++ )
        {
            if ( (*listas_curvas[i])[j]->maxYValue() > max )
                max = (*listas_curvas[i])[j]->maxYValue();
        }
    }

    return max;
}
        

void Grafico :: adicionar_conjunto( const QString &nome_conj )
{
    if ( conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Já existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    conjuntos << nome_conj;

    // Lista de curvas
    QList< QwtPlotCurve * > *curvas = new QList< QwtPlotCurve * >;

    map_conj_curvas[ nome_conj ] = curvas;

    // Lista de nomes de curvas
    QStringList *nomes_curvas = new QStringList;

    map_conj_nomes_curvas[ nome_conj ] = nomes_curvas;

    // Lista de nomes das detecções
    QStringList *nomes_detec = new QStringList;

    map_conj_nomes_detec[ nome_conj ] = nomes_detec;

    // Lista de dados (x, y)
    QList< ParXY * > *lista_dados = new QList< ParXY * >;

    map_conj_dados[ nome_conj ] = lista_dados;

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        // TODO
    }
}


// Nesta função uma nova curva é adicionada ao conjunto se não existir nenhuma
// curva com o mesmo nome. Ao adicionar a nova curva, a lista de nomes das
// curvas e a lista de curvas será atualizada de tal maneira que o nome terá o
// mesmo índice que a curva em suas respectivas listas. Isso fará com que a
// curva possa ser indexada pelo mesmo índice do nome da curva.
void Grafico :: adicionar_curva( const QString &nome_conj, 
                                 const QString &nome_curva,
                                 const QPen &linha,
                                 const QwtPlotCurve::CurveStyle &estilo )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    // Obtendo a lista de nomes de curvas do dado conjunto
    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    if ( nomes_curvas->contains( nome_curva ) )
    {
        throw ExcecaoCurva( "Já existe uma curva com o nome <b>" + nome_curva +
                            "</b> adicionada à lista de curvas do conjunto <b>" 
                            + nome_conj + "</b>." );
    }

    // Adicionando o nome da nova curva à lista de nomes
    (*nomes_curvas) << nome_curva;

    // Obtendo a lista de curvas do dado conjunto
    QList< QwtPlotCurve * > *curvas = map_conj_curvas[ nome_conj ];

    // Criando a nova curva
    QwtPlotCurve *curva = new QwtPlotCurve( nome_curva );

    curva->setPen( linha );
    curva->setStyle( estilo );

    (*curvas) << curva;

    curva->attach( this );

    // Criando o par (x, y) e adicionando-o à lista de dados
    QList< ParXY * > *lista_dados = map_conj_dados[ nome_conj ];

    ParXY *par = new ParXY;

    (*lista_dados) << par;

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        // TODO
    }
}


void Grafico :: adicionar_deteccao( const QString &nome_conj,
                                    const QString &nome_detec )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    // Obtendo a lista de nomes das detecções do dado conjunto
    QStringList *nomes_detec = map_conj_nomes_detec[ nome_conj ];

    if ( nomes_detec->contains( nome_detec ) )
    {
        throw ExcecaoDeteccao( "Já existe uma detecção com o nome <b>" + 
                               nome_detec + "</b> adicionada à lista de "
                               "detecções do conjunto <b>" + nome_conj + 
                               "</b>." );
    }

    // Adicionando o nome da nova curva à lista de nomes
    (*nomes_detec) << nome_detec;

    // Mapeando uma lista de retangulos a partir de um par (conjunto,nome_detec)
    QList< Retangulo * > *lista = new QList< Retangulo * >;

    QPair< QString, QString > conj_detec;

    conj_detec.first = nome_conj;
    conj_detec.second = nome_detec;

    map_conj_detec_retangulos[ conj_detec ] = lista;
}


void Grafico :: adicionar_intervalo_detec( const QString &nome_conj,
                                           const QString &nome_detec,
                                           const QPair< double, 
                                                        double > &inicio_fim,
                                           const QPen &linha,
                                           const QBrush &preenchimento,
                                           const bool &replote )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    // Obtendo a lista de nomes das detecções do dado conjunto
    QStringList *nomes_detec = map_conj_nomes_detec[ nome_conj ];

    if ( nomes_detec->contains( nome_detec ) )
    {
        throw ExcecaoDeteccao( "Já existe uma detecção com o nome <b>" + 
                               nome_detec + "</b> adicionada à lista de "
                               "detecções do conjunto <b>" + nome_conj + 
                               "</b>." );
    }

    // Obtendo a lista de retangulos a partir do par (nome_conj, nome_detec)
    QPair< QString, QString > conj_detec;

    conj_detec.first = nome_conj;
    conj_detec.second = nome_detec;

    QList< Retangulo * > *lista = map_conj_detec_retangulos[ conj_detec ];

    // Criando o novo retangulo e adicionando-o a lista/grafico
    Retangulo *retangulo = new Retangulo( linha, preenchimento );

    qreal x_inicial = inicio_fim.first;
    qreal y_inicial = min_y();
    qreal larg = inicio_fim.second - x_inicial;
    qreal alt = max_y() - y_inicial;

    retangulo->configurar( QRectF( x_inicial, y_inicial, larg, alt ) );

    retangulo->attach( this );

    (*lista) << retangulo;

    // TODO parei aqui

    if ( replote )
        this->replot();
}


void Grafico :: adicionar_xy( const QString &nome_conj, 
                              const QString &nome_curva,
                              const double &x,
                              const double &y,
                              const bool &replote )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    // Obtendo a lista de nomes de curvas para o dado conjunto
    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    int indice_curva = nomes_curvas->indexOf( nome_curva );

    if ( indice_curva == -1 )
    {
        throw ExcecaoCurva( "Não existe uma curva com o nome <b>" + nome_curva +
                            "</b> adicionada à lista de curvas do conjunto <b>" 
                            + nome_conj + "</b>." );
    }

    // Obtendo os vetores X e Y da lista de pares
    ParXY *dados = (*map_conj_dados[ nome_conj ])[ indice_curva ];

    // Adicionando o valor de X
    dados->first.push_back( x );
    // Adicionando o valor de Y
    dados->second.push_back( y );

    // Obtendo a curva a da lista de curvas
    QwtPlotCurve *curva = (*map_conj_curvas[ nome_conj ])[ indice_curva ];

    curva->setRawSamples( dados->first.data(), 
                          dados->second.data(), 
                          dados->first.count() );

    if ( replote )
        this->replot();
}


void Grafico :: atualizar()
{
    this->replot();
}


void Grafico :: configurar_legenda( Legenda *l )
{
    legenda = l;
}


void Grafico :: habilitar_legenda( const bool &b )
{
    legenda->setVisible( b );
}


void Grafico :: habilitar_zoom( const bool &b )
{
    zoom->setEnabled( b );
}


void Grafico :: limpar( const bool &replote )
{
    while( !conjuntos.isEmpty() )
    {
        remover_conjunto( conjuntos.first(), false );
    }

    if ( replote )
        this->replot();
}


void Grafico :: remover_conjunto( const QString &nome_conj, 
                                  const bool &replote )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    while( !nomes_curvas->isEmpty() )
    {
        remover_curva( nome_conj, nomes_curvas->first(), false );
    }

    conjuntos.removeOne( nome_conj );

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        // TODO
    }

    if ( replote )
        this->replot();
}


void Grafico :: remover_curva( const QString &nome_conj, 
                               const QString &nome_curva,
                               const bool &replote )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    // Obtendo a lista de nomes de curvas para o dado conjunto
    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    int indice_curva = nomes_curvas->indexOf( nome_curva );

    if ( indice_curva == -1 )
    {
        throw ExcecaoCurva( "Não existe uma curva com o nome <b>" + nome_curva +
                            "</b> adicionada à lista de curvas do conjunto <b>" 
                            + nome_conj + "</b>." );
    }

    delete (*map_conj_curvas[ nome_conj ])[ indice_curva ];
    delete (*map_conj_dados[ nome_conj ])[ indice_curva ];

    map_conj_curvas[ nome_conj ]->removeAt( indice_curva );
    map_conj_dados[ nome_conj ]->removeAt( indice_curva );

    nomes_curvas->removeAt( indice_curva );

    if ( legenda != NULL )
    {
        // TODO
    }

    if ( replote )
        this->replot();
}


void Grafico :: remover_curvas( const QString &nome_conj, 
                                const bool &replote )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
    }

    // Obtendo a lista de nomes de curvas para o dado conjunto
    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    while( !nomes_curvas->isEmpty() )
    {
        delete (*map_conj_curvas[ nome_conj ])[ 0 ];
        delete (*map_conj_dados[ nome_conj ])[ 0 ];

        map_conj_curvas[ nome_conj ]->removeAt( 0 );
        map_conj_dados[ nome_conj ]->removeAt( 0 );
        
        nomes_curvas->removeAt( 0 );
    }

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        // TODO
    }

    if ( replote )
        this->replot();
}


void Grafico :: remover_deteccoes( const QString &nome_conj, 
                                   const bool &replote )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        throw ExcecaoConjunto( "Não existe um conjunto com o nome de <b>" +
                               nome_conj + "</b> adicionado à lista de "
                               "conjuntos." );
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

#endif
