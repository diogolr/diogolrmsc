#ifndef LEGENDA_H_
#define LEGENDA_H_

#include <QCheckBox>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHash>
#include <QHBoxLayout>
#include <QList>
#include <QObject>
#include <QString>
#include <QWidget>

#include "qwt_plot.h"
#include "qwt_plot_curve.h"
#include "qwt_text.h"

#include "ui_legenda.h"

class Legenda : public QWidget
{
    Q_OBJECT

    // Metodos
    public:
        Legenda( QWidget *pai = 0 );
        ~Legenda();

        void adicionar_item( QwtPlotCurve * );
        void associar_grafico( QwtPlot * );
        void limpar();

    private:
        void inicializar();

    private slots:
        void mudanca_estado();

    // Atributos
    private:
        Ui_Legenda *ui;

        QHash< QCheckBox *, QwtPlotCurve * > mapeamento_cb_curva;
        
        QList< QHBoxLayout * > layouts;

        QwtPlot *grafico;
};

#endif
