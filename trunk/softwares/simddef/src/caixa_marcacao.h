#ifndef CAIXA_MARCACAO_H_
#define CAIXA_MARCACAO_H_

#include <QGraphicsScene>
#include <QRectF>
#include <QString>
#include <QWidget>

#include <qwt_plot_curve.h>
#include <qwt_text.h>

#include "funcoes.h"
#include "retangulo.h"
#include "ui_caixa_marcacao.h"

class CaixaMarcacao : public QWidget
{
    Q_OBJECT

    // Atributos publicos
    public:
        enum TipoItem{ Curva = 0, Deteccao };

    // Metodos
    public:
        CaixaMarcacao( QwtPlotCurve * );
        CaixaMarcacao( const QString &, 
                       const QList< Retangulo * > & );
        ~CaixaMarcacao();

    private:
        void inicializa_curva( QwtPlotCurve * );
        void inicializa_deteccao( const QString &,
                                  const QList< Retangulo * > & );

    private slots:
        void on_cb_stateChanged( int );

    signals:
        void item_atualizado();

    // Atributos
    private:
        QList< Retangulo * > retangulos;

        QwtPlotCurve *curva;

        TipoItem tipo_item;
        
        Ui_CaixaMarcacao *ui;
};

#endif
