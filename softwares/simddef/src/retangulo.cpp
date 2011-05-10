#ifndef RETANGULO_CPP_
#define RETANGULO_CPP_

#include "retangulo.h"

Retangulo :: Retangulo( const QPen &p, const QBrush &b ) : 
             preenchimento( b ), linha( p )
{
}


Retangulo :: ~Retangulo()
{
}


void Retangulo :: configurar_retangulo( const QRectF &ret )
{
    coordenadas = ret;
}


void Retangulo :: configurar_linha( const QPen &l )
{
    linha = l;
}


void Retangulo :: configurar_preenchimento( const QBrush &b )
{
    preenchimento = b;
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

        painter->setPen( linha );
        painter->setBrush( preenchimento );

        QwtPainter::drawRect( painter, rect );
    }
}

#endif
