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


#include <iostream>
using std::cout;
void JanelaPrincipal :: on_botao_clicked()
{
    ConfigFalha cfg;

    cfg.carregar_falhas( "falhas.sdd" );

    cfg.exec();
}

#endif
