#ifndef RETANGULO_CPP_
#define RETANGULO_CPP_

#include "retangulo.h"

Retangulo :: Retangulo( const QPen &p, const QBrush &b ) : 
             estilo_preenchimento( b ), estilo_linha( p )
{
}


Retangulo :: ~Retangulo()
{
}


QBrush Retangulo :: preenchimento()
{
    return estilo_preenchimento;
}


QPen Retangulo :: linha()
{
    return estilo_linha;
}


void Retangulo :: configurar_retangulo( const QRectF &ret )
{
    coordenadas = ret;
}


void Retangulo :: configurar_linha( const QPen &l )
{
    estilo_linha = l;
}


void Retangulo :: configurar_preenchimento( const QBrush &b )
{
    estilo_preenchimento = b;
}


void Retangulo :: draw( QPainter *painter, 
                        const QwtScaleMap &x_map,
                        const QwtScaleMap &y_map,
                        const QRectF &canvas_rect ) const
{
    Q_UNUSED( canvas_rect );

    if ( coordenadas.isValid() )
    {
        const QRectF rect = QwtScaleMap::transform( x_map, y_map, coordenadas );

        painter->setPen( estilo_linha );
        painter->setBrush( estilo_preenchimento );

        QwtPainter::drawRect( painter, rect );
    }
}

#endif
