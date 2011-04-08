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

#endif
