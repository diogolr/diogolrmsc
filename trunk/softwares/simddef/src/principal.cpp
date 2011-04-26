#ifndef PRINCIPAL_CPP_
#define PRINCIPAL_CPP_

#include "principal.h"

JanelaPrincipal :: JanelaPrincipal()
{
    ui = new Ui_JanelaPrincipal;

    ui->setupUi( this );
}


JanelaPrincipal :: ~JanelaPrincipal()
{
    delete ui;
}


void JanelaPrincipal :: on_botao_clicked()
{
    ConfigFalha cfg( "../cfgs/falhas.sdd", this );

    cfg.ler_falhas();

    cfg.exec();
}

#endif
