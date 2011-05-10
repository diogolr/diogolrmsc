#ifndef RETANGULO_H_
#define RETANGULO_H_

#include <QBrush>
#include <QPainter>
#include <QPen>
#include <QRectF>

#include <qwt_painter.h>
#include <qwt_plot_item.h>
#include <qwt_scale_map.h>

class Retangulo : public QwtPlotItem
{
    // Metodos
    public:
        Retangulo( const QPen &, const QBrush & );
        ~Retangulo();

        void configurar_retangulo( const QRectF & );
        void configurar_linha( const QPen & );
        void configurar_preenchimento( const QBrush & );

        void draw( QPainter *, 
                   const QwtScaleMap &,
                   const QwtScaleMap &,
                   const QRectF & ) const;

    // Atributos
    private:
        QBrush preenchimento;
        QPen linha;
        QRectF coordenadas;
};

#endif
