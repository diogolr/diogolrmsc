#ifndef GRAFICO_H_
#define GRAFICO_H_

#include <QColor>
#include <QHash>
#include <QLinearGradient>
#include <QList>
#include <QPair>
#include <QPalette>
#include <QPen>
#include <QString>
#include <QVector>
#include <QWidget>

#include <qwt_picker.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_zoomer.h>

typedef QVector< double > QVectorD;
typedef QPair< QVectorD, QVectorD > ParXY;

#include "funcoes.h"
#include "legenda.h"

class Grafico : public QwtPlot
{
    // Metodos
    public:
        Grafico( QWidget *pai = NULL );
        ~Grafico();

        bool adicionar_conjunto( const QString & );
        bool adicionar_curva( const QString &, 
                              const QString &,
                              const QPen & = QPen(),
                              const QwtPlotCurve::CurveStyle & = 
                                    QwtPlotCurve::Lines );

        void adicionar_xy( const QString &, 
                           const QString &,
                           const double &,
                           const double &,
                           const bool & = true );
        void configurar_legenda( Legenda * );
        void habilitar_legenda( const bool & );
        void habilitar_zoom( const bool & );
        void limpar( const bool & = true );
        void remover_conjunto( const QString &, const bool & = true );
        void remover_curva( const QString &, 
                            const QString &, 
                            const bool & = true );

    private:
        void configurar();
        void inicializar();

    // Atributos
    private:
        QWidget *pai;

        Legenda *legenda;

        // Conjuntos de curvas
        QStringList conjuntos;

        // Hash de conjuntos para curvas
        QHash< QString, QList< QwtPlotCurve * > * > map_conj_curvas;
        QHash< QString, QStringList * > map_conj_nomes_curvas;
        QHash< QString, QList< ParXY * > * > map_conj_dados;

        QwtPlotZoomer *zoom;
};

#endif
