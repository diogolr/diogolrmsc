#ifndef CONJUNTO_ITENS_CPP_
#define CONJUNTO_ITENS_CPP_

#include "grafico.h"

ConjuntoItens :: ConjuntoItens( Grafico *g, const QString &nome ) :
                 grafico( g ), nome_conj( nome )
{
}


ConjuntoItens :: ~ConjuntoItens()
{
}


void ConjuntoItens :: adicionar_curva( const QString &nome_curva, 
                                       const QPen &linha,
                                       const QwtPlotCurve::CurveStyle &estilo )
{
    if ( nomes_curvas.contains( nome_curva ) )
    {
        throw ExcecaoCurva( "Já existe uma curva com o nome <b>" + nome_curva +
                            "</b> adicionada à lista de curvas do conjunto <b>" 
                            + nome_conj + "</b>." );
    }

    nomes_curvas << nome_curva;

    QwtPlotCurve *nova_curva = new QwtPlotCurve( nome_curva );

    nova_curva->setPen( linha );
    nova_curva->setStyle( estilo );
    nova_curva->attach( grafico );

    curvas << nova_curva;

    XY valores;

    dados << valores;
}


void ConjuntoItens :: adicionar_deteccao( const QString &nome_detec )
{
    if ( nomes_deteccoes.contains( nome_detec ) )
    {
        throw ExcecaoDeteccao( "Já existe uma detecção com o nome <b>" + 
                               nome_detec + "</b> adicionada à lista de "
                               "curvas do conjunto <b>" + nome_conj + "</b>." );
    }

    nomes_deteccoes << nome_detec;

    QList< Retangulo * > *lista_retangulos = new QList< Retangulo * >;

    map_detec_retangulos[ nome_detec ] = lista_retangulos;
}


void ConjuntoItens :: adicionar_intervalo_detec( const QString &nome_detec,
                                                 const QRectF &ret,
                                                 const QPen &linha,
                                                 const QBrush &preenchimento )
{
    int indice_detec = nomes_deteccoes.indexOf( nome_detec );

    if ( indice_detec == -1 )
    {
        throw ExcecaoDeteccao( "Não existe uma detecção com o nome <b>" + 
                               nome_detec + "</b> adicionada à lista de "
                               "detecções do conjunto <b>" + nome_conj + 
                               "</b>." );
    }

    // Obtendo a lista de retangulos para removê-los do gráfico
    QList< Retangulo * > *retangulos;

    retangulos = map_detec_retangulos[ nomes_deteccoes[indice_detec] ];

    Retangulo *retangulo = new Retangulo( linha, preenchimento );

    retangulo->configurar_retangulo( ret );
    retangulo->attach( grafico );

    (*retangulos) << retangulo;
}


void ConjuntoItens :: adicionar_xy( const QString &nome_curva,
                                    const double &x, 
                                    const double &y )
{
    int indice_curva = nomes_curvas.indexOf( nome_curva );

    if ( indice_curva == - 1 )
    {
        throw ExcecaoCurva( "Não existe uma curva com o nome <b>" + nome_curva +
                            "</b> adicionada à lista de curvas do conjunto <b>" 
                            + nome_conj + "</b>." );
    }

    // Adicionando o valor de X
    dados[indice_curva].first.push_back( x );
    // Adicionando o valor de Y
    dados[indice_curva].second.push_back( y );

    // Configurando a curva com os novos dados
    curvas[indice_curva]->setRawSamples( dados[indice_curva].first.data(), 
                                         dados[indice_curva].second.data(), 
                                         dados[indice_curva].first.count() );
}


void ConjuntoItens :: limpar()
{
    remover_curvas();
    remover_deteccoes();
}


void ConjuntoItens :: remover_curva( const QString &nome_curva )
{
    int indice = nomes_curvas.indexOf( nome_curva );

    if ( indice == -1 )
    {
        throw ExcecaoCurva( "Não existe uma curva com o nome <b>" + nome_curva +
                            "</b> adicionada à lista de curvas do conjunto <b>" 
                            + nome_conj + "</b>." );
    }

    curvas[indice]->detach();

    curvas.removeAt( indice );
    dados.removeAt( indice );
    nomes_curvas.removeAt( indice );
}


void ConjuntoItens :: remover_curvas()
{
    // Limpando as curvas
    QwtPlotCurve *curva;

    while( !curvas.isEmpty() )
    {
        curva = curvas.takeFirst();

        curva->detach();

        delete curva;
    }

    curvas.clear();
    dados.clear();
    nomes_curvas.clear();
}


void ConjuntoItens :: remover_deteccao( const QString &nome_detec )
{
    int indice_detec = nomes_deteccoes.indexOf( nome_detec );

    if ( indice_detec == -1 )
    {
        throw ExcecaoDeteccao( "Não existe uma detecção com o nome <b>" + 
                               nome_detec + "</b> adicionada à lista de "
                               "detecções do conjunto <b>" + nome_conj + 
                               "</b>." );
    }

    // Obtendo a lista de retangulos para removê-los do gráfico
    QList< Retangulo * > *retangulos;

    retangulos = map_detec_retangulos[ nomes_deteccoes[indice_detec] ];

    Retangulo * ret;

    while( !retangulos->isEmpty() )
    {
        ret = retangulos->takeFirst();

        ret->detach();

        delete ret;
    }

    delete retangulos;

    map_detec_retangulos.remove( nomes_deteccoes[indice_detec] );

    nomes_deteccoes.removeAt( indice_detec );
}


void ConjuntoItens :: remover_deteccoes()
{
    QList< Retangulo * > *retangulos;

    Retangulo *ret;

    for ( int i = 0 ; i < nomes_deteccoes.count() ; i++ )
    {
        retangulos = map_detec_retangulos[ nomes_deteccoes[i] ];

        while( !retangulos->isEmpty() )
        {
            ret = retangulos->takeFirst();

            ret->detach();

            delete ret;
        }
    
        delete retangulos;
    }

    nomes_deteccoes.clear();
    map_detec_retangulos.clear();
}

#endif
