#ifndef CAIXA_MARCACAO_H_
#define CAIXA_MARCACAO_H_

#include <QCheckBox>
#include <QString>
#include <QWidget>

#include "ui_caixa_marcacao.h"

class CaixaMarcacao : public QCheckBox
{
    Q_OBJECT

    // Metodos
    public:
        CaixaMarcacao( QWidget *pai = NULL );
        ~CaixaMarcacao();

        bool marcado();

        void configurar_texto( const QString & );

    private slots:
        void on_cb_stateChanged( int );

    signals:
        void mudanca_estado( bool );

    // Atributos
    private:
        Ui_CaixaMarcacao *ui;
};

#endif
