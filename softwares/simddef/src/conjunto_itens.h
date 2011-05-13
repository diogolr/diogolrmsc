#ifndef CONJUNTO_ITENS_H_
#define CONJUNTO_ITENS_H_

#include <QBrush>
#include <QHash>
#include <QList>
#include <QPair>
#include <QPen>
#include <QRectF>
#include <QString>
#include <QStringList>
#include <QVector>

#include <qwt_plot_curve.h>

#include "grafico.h"
#include "excecoes.h"
#include "retangulo.h"

typedef QVector< double > QVectorD;
typedef QPair< QVectorD, QVectorD > XY;

class ConjuntoItens
{
    // Metodos
    public:
        ConjuntoItens( Grafico *, const QString & );
        ~ConjuntoItens();

        QList< Retangulo * > * retangulos( const QString & );

        QPair< QPen, QBrush > estilos( const QString & );

        QStringList deteccoes();

        QwtPlotCurve * curva( const QString & );

        void adicionar_curva( const QString &,
                              const QPen & = QPen(),
                              const QwtPlotCurve::CurveStyle & =
                                    QwtPlotCurve::Lines );
        void adicionar_deteccao( const QString &, 
                                 const QPen &, 
                                 const QBrush & );
        void adicionar_xy( const QString &, 
                           const double &, 
                           const double & );
        void adicionar_intervalo_detec( const QString &, 
                                        const QRectF & );
        void limpar();
        void remover_curva( const QString & );
        void remover_curvas();
        void remover_deteccao( const QString & );
        void remover_deteccoes();

    // Atributos
    private:
        Grafico *grafico;
        
        QHash< QString, QPair< QPen, QBrush > > map_detec_estilos;
        QHash< QString, QList< Retangulo * > * > map_detec_retangulos;

        QString nome_conj;

        QStringList nomes_curvas;
        QStringList nomes_deteccoes;

        QList< QwtPlotCurve * > curvas;
        QList< XY > dados;
};

#endif
