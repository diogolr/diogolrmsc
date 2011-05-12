#ifndef LEGENDA_H_
#define LEGENDA_H_

#include <QBrush>
#include <QFrame>
#include <QHash>
#include <QList>
#include <QPair>
#include <QPen>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include <qwt_plot_curve.h>
#include <qwt_plot_item.h>
#include <qwt_text.h>

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
        void adicionar_deteccao( const QString &, 
                                 const QString &, 
                                 const QPair< QPen, QBrush > &,
                                 QList< Retangulo * > *retangulos );
        void limpar();
        void remover_conjunto( const QString & );
        /*
        void remover_curva( const QString &, const QString & );
        void remover_deteccao( const QString &, const QString & );
        */

    private:
        void inicializar();

    private slots:
        void item_legenda_atualizado();

    signals:
        void legenda_atualizada();

    // Atributos
    private:
        QHash< QString, QVBoxLayout * > map_conj_layout;
        QHash< QString, QWidget * > map_conj_widget;
        /*
        QHash< QPair< QString, QString >, CaixaMarcacao * > map_conj_curva_caixa_marc;
        QHash< QPair< QString, QString >, CaixaMarcacao * > map_conj_detec_caixa_marc;
        */

        Ui_Legenda *ui;
};

#endif
