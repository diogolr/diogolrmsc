#ifndef GRAFICO_H_
#define GRAFICO_H_

#include <QDebug>

#include <QChar>
#include <QFont>
#include <QPen>
#include <QPixmap>
#include <QSize>
#include <QString>
#include <QSvgGenerator>
#include <QVector>
#include <QWidget>

#include <qwt_legend.h>
#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_curve.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_item.h>
#include <qwt_plot_layout.h>
#include <qwt_plot_printfilter.h>
#include <qwt_plot_zoomer.h>
#include <qwt_picker.h>
#include <qwt_text.h>

#include <fstream>
using std::ifstream;
using std::ofstream;
using std::ios;
#include <iostream>
using std::endl;

#include "funcoes.h"

class Grafico : public QwtPlot
{
    // Metodos
    public:
        Grafico( QWidget *pai = NULL );
        ~Grafico();

        QwtPlotCurve * curva( const int & );

        void adicionar_curva( const QPen &, const QString & = "" );
        void adicionar_xy( const int &curva, 
                           const double &x, 
                           const double &y,
                           const bool &replote = true );
        void configurar_legenda( QwtLegend *, const QwtPlot::LegendPosition & );
        void configurar_limites( const QwtPlot::Axis &, 
                                 const double &,
                                 const double & );
        void configurar_nome_eixo( const QwtPlot::Axis &, 
                                   const QwtText & );
        void configurar_titulo( const QwtText & );
        void exportar( const QString &nome_arquivo = "saida.svg" );
        void habilitar_zoom( const bool & );
        void limpar( const int &curva = -1 );
        void salvar( const QString &nome_arquivo = "saida.dat", 
                     const char &separador = '\t',
                     const int &curva = -1,
                     const bool &msm_valor_x = true );
        void utilizar_grid( const bool & );

    private:
        void configurar();

    // Atributos
    private:
        QVector< QVector< double > * > x;
        QVector< QVector< double > * > y;

        QVector< QwtPlotCurve * > curvas;

        QWidget *pai;

        QwtLegend *legenda;
        QwtPlotGrid grid;
        QwtPlotZoomer *zoom;
};

#endif
