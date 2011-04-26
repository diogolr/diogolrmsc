#ifndef PRINCIPAL_CPP_
#define PRINCIPAL_CPP_

#include "principal.h"

JanelaPrincipal :: JanelaPrincipal()
{
    ui = new Ui_JanelaPrincipal;

    ui->setupUi( this );

    inicializar();
}


JanelaPrincipal :: ~JanelaPrincipal()
{
    if ( cfg_falhas != NULL )
        delete cfg_falhas;

    if ( cfg_modulo != NULL )
        delete cfg_modulo;

    delete ui;
}


void JanelaPrincipal :: atualizar_falhas()
{
}


void JanelaPrincipal :: inicializar()
{
    abreviaturas.clear();
    descricoes.clear();

    cfg_falhas = NULL;
    cfg_modulo = NULL;
}


void JanelaPrincipal :: on_acao_cfg_falhas_triggered()
{
    if ( cfg_falhas != NULL )
        delete cfg_falhas;

    cfg_falhas = new ConfigFalhas( this, ui->end_arq_falhas->text() );

    cfg_falhas->exec();

    abreviaturas = cfg_falhas->abreviaturas();
    descricoes = cfg_falhas->descricoes();
}


void JanelaPrincipal :: on_botao_carregar_falhas_clicked()
{
    QString nome_arq = QFileDialog::getOpenFileName( this, 
                                                     "Abrir arquivo...",
                                                     ".",
                                                   "Arquivos Simddef (*.sdd)" );

    if ( !nome_arq.isEmpty() )
    {
        ui->end_arq_falhas->setText( nome_arq );

        ui->acao_cfg_falhas->setEnabled( true );
    }
}


void JanelaPrincipal :: on_botao_clicked()
{
}

#endif
