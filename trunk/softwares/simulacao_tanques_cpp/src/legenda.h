#ifndef LEGENDA_H_
#define LEGENDA_H_

#include <QCheckBox>
#include <QGraphicsLineItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QObject>
#include <QString>
#include <QVector>
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

    private:
        void inicializar();

    private slots:
        void mudanca_estado();

    // Atributos
    private:
        Ui_Legenda *ui;
        
        QVector< QString > nomes_itens;
        QVector< QGraphicsView * > imagens;
        QVector< QHBoxLayout * > layouts;
        QVector< QwtPlotCurve * > curvas;

        QwtPlot *grafico;
};

#endif
