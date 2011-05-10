#ifndef GRAFICO_H_
#define GRAFICO_H_

#include <QBrush>
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

#include <cfloat>

#include <qwt_picker.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_zoomer.h>
#include <qwt_scale_div.h>

typedef QVector< double > QVectorD;
typedef QPair< QVectorD, QVectorD > ParXY;

#include "excecoes.h"
#include "funcoes.h"
#include "legenda.h"
#include "retangulo.h"

class Grafico : public QwtPlot
{
    // Metodos
    public:
        Grafico( QWidget *pai = NULL );
        ~Grafico();

        double min_x();
        double min_y();
        double max_x();
        double max_y();

        void adicionar_conjunto( const QString & );
        void adicionar_curva( const QString &, 
                              const QString &,
                              const QPen & = QPen(),
                              const QwtPlotCurve::CurveStyle & = 
                                    QwtPlotCurve::Lines );
        void adicionar_deteccao( const QString &, const QString & );
        void adicionar_intervalo_detec( const QString &,
                                        const QString &,
                                        const QPair< double, double > &,
                                        const QPen & = QPen(),
                                        const QBrush & = QBrush(),
                                        const bool & = true );
        void adicionar_xy( const QString &, 
                           const QString &,
                           const double &,
                           const double &,
                           const bool & = true );
        void atualizar();
        void configurar_legenda( Legenda * );
        void habilitar_legenda( const bool & );
        void habilitar_zoom( const bool & );
        void limpar( const bool & = true );
        void remover_conjunto( const QString &, const bool & = true );
        void remover_curva( const QString &, 
                            const QString &, 
                            const bool & = true );
        void remover_curvas( const QString &, const bool & = true );
        void remover_deteccao( const QString &, 
                               const QString &, 
                               const bool & = true );
        void remover_deteccoes( const QString &, const bool & = true );

    private:
        void configurar();
        void inicializar();

    // Atributos
    private:
        Legenda *legenda;

        // Conjuntos de curvas
        QStringList conjuntos;

        // Hash de conjuntos para curvas
        QHash< QString, QList< QwtPlotCurve * > * > map_conj_curvas;
        QHash< QString, QStringList * > map_conj_nomes_curvas;
        QHash< QString, QStringList * > map_conj_nomes_detec;
        QHash< QString, QList< ParXY * > * > map_conj_dados;
        QHash< QPair< QString, QString >, 
               QList< Retangulo * > * > map_conj_detec_retangulos;

        QWidget *pai;

        QwtPlotZoomer *zoom;
};

#endif
