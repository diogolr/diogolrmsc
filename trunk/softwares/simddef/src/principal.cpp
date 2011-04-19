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
    Rede r;
    switch( config.exec() )
    {
        case QDialog::Accepted:
            r.ler_arquivos( config.arquivos() );
            break;
        case QDialog::Rejected:
            break;
    }

    Matrix< double > saida = r.executar();

    cout << saida;
}

#endif
