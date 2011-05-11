#ifndef LEGENDA_H_
#define LEGENDA_H_

#include <QFrame>
#include <QHash>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>

#include "caixa_marcacao.h"
#include "retangulo.h"
#include "ui_legenda.h"

class Legenda : public QWidget
{
    Q_OBJECT

    // Metodos
    public:
        Legenda( QWidget *pai = NULL );
        ~Legenda();

        void adicionar_conjunto( const QString & );
        void adicionar_curva( const QString &, QwtPlotCurve * );
        void adicionar_deteccao( const QString &, Retangulo * );
        void limpar();

    private:
        void inicializar();

    private slots:
        void item_legenda_atualizado();

    signals:
        void legenda_atualizada();

    // Atributos
    private:
        QHash< QString, QVBoxLayout * > map_conj_layout;

        Ui_Legenda *ui;
};

#endif
