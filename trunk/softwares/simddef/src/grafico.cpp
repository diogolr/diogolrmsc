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
        

bool Grafico :: adicionar_conjunto( const QString &nome_conj )
{
    if ( conjuntos.contains( nome_conj ) )
    {
        exibir_mensagem( pai, 
                         "Erro", 
                         "Já existe um conjunto com o nome de <b>" +
                         nome_conj + "</b> adicionado à lista de conjuntos.", 
                         Aviso );
        return false;
    }

    conjuntos << nome_conj;

    // Lista de curvas
    QList< QwtPlotCurve * > *curvas = new QList< QwtPlotCurve * >;

    map_conj_curvas[ nome_conj ] = curvas;

    // Lista de nomes de curvas
    QStringList *nomes_curvas = new QStringList;

    map_conj_nomes_curvas[ nome_conj ] = nomes_curvas;

    // Lista de dados (x, y)
    QList< ParXY * > *lista_dados = new QList< ParXY * >;

    map_conj_dados[ nome_conj ] = lista_dados;

    // Atualizando a legenda
    if ( legenda != NULL )
    {
        // TODO
    }

    return true;
}


// Nesta função uma nova curva é adicionada ao conjunto se não existir nenhuma
// curva com o mesmo nome. Ao adicionar a nova curva, a lista de nomes das
// curvas e a lista de curvas será atualizada de tal maneira que o nome terá o
// mesmo índice que a curva em suas respectivas listas. Isso fará com que a
// curva possa ser indexada pelo mesmo índice do nome da curva.
bool Grafico :: adicionar_curva( const QString &nome_conj, 
                                 const QString &nome_curva,
                                 const QPen &linha,
                                 const QwtPlotCurve::CurveStyle &estilo )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        exibir_mensagem( pai, 
                         "Erro", 
                         "Não existe um conjunto com o nome de <b>" +
                         nome_conj + "</b> adicionado à lista de conjuntos.", 
                         Aviso );
        return false;
    }

    // Obtendo a lista de nomes de curvas do dado conjunto
    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    if ( nomes_curvas->contains( nome_curva ) )
    {
        exibir_mensagem( pai, 
                         "Erro", 
                         "Já existe uma curva com o nome <b>" + nome_curva + 
                         "</b> adicionada à lista de curvas do conjunto <b>" +
                         nome_conj + "</b>.", 
                         Aviso );
        return false;
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

    return true;
}


void Grafico :: adicionar_xy( const QString &nome_conj, 
                              const QString &nome_curva,
                              const double &x,
                              const double &y,
                              const bool &replote )
{
    if ( !conjuntos.contains( nome_conj ) )
    {
        exibir_mensagem( pai, 
                         "Erro", 
                         "Não existe um conjunto com o nome de <b>" +
                         nome_conj + "</b> adicionado à lista de conjuntos.", 
                         Aviso );
        return;
    }

    // Obtendo a lista de nomes de curvas para o dado conjunto
    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    int indice_curva = nomes_curvas->indexOf( nome_curva );

    if ( indice_curva == -1 )
    {
        exibir_mensagem( pai, 
                         "Erro", 
                         "Não existe uma curva com o nome <b>" + nome_curva + 
                         "</b> adicionada à lista de curvas do conjunto <b>" +
                         nome_conj + "</b>.", 
                         Aviso );
        return;
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
        exibir_mensagem( pai, 
                         "Erro", 
                         "Não existe um conjunto com o nome de <b>" +
                         nome_conj + "</b> adicionado à lista de conjuntos.", 
                         Aviso );
        return;
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
        exibir_mensagem( pai, 
                         "Erro", 
                         "Não existe um conjunto com o nome de <b>" +
                         nome_conj + "</b> adicionado à lista de conjuntos.", 
                         Aviso );
        return;
    }

    // Obtendo a lista de nomes de curvas para o dado conjunto
    QStringList *nomes_curvas = map_conj_nomes_curvas[ nome_conj ];

    int indice_curva = nomes_curvas->indexOf( nome_curva );

    if ( indice_curva == -1 )
    {
        exibir_mensagem( pai, 
                         "Erro", 
                         "Não existe uma curva com o nome <b>" + nome_curva + 
                         "</b> adicionada à lista de curvas do conjunto <b>" +
                         nome_conj + "</b>.", 
                         Aviso );
        return;
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
