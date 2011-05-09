#ifndef CAIXA_MARCACAO_CPP_
#define CAIXA_MARCACAO_CPP_

#include "caixa_marcacao.h"

CaixaMarcacao :: CaixaMarcacao( QWidget *pai ) : QCheckBox( pai )
{
    ui = new Ui_CaixaMarcacao;

    ui->setupUi( this );
}


CaixaMarcacao :: ~CaixaMarcacao()
{
    delete ui;
}


bool CaixaMarcacao :: marcado()
{
    bool flag = false;

    switch( ui->cb->checkState() )
    {
        case Qt::PartiallyChecked:
        case Qt::Checked:
            flag = true;
            break;

        case Qt::Unchecked:
            break;
    }

    return flag;
}


void CaixaMarcacao :: configurar_texto( const QString &str )
{
    ui->label->setText( str );
}


void CaixaMarcacao :: on_cb_stateChanged( int estado )
{
    Q_UNUSED( estado );

    emit( mudanca_estado( marcado() ) );
}

#endif
