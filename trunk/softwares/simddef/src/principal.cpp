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

    if ( cfg_modulos != NULL )
        delete cfg_modulos;

    delete ui;
}


void JanelaPrincipal :: atualizar_falhas( const QString &nome_arq )
{
    try
    {
        lista_falhas = manipulador_xml.ler_falhas( nome_arq );
    }
    catch( ExcecaoArquivo e )
    {
        throw e;
    }
    catch( ExcecaoLeituraXML e )
    {
        throw e;
    }

    // Atualizando a lista de abreviaturas e descricoes e a exibicao das falhas
    // a serem detectadas
    QTreeWidgetItem *atuadores = ui->falhas->topLevelItem( 0 );
    QTreeWidgetItem *sensores = ui->falhas->topLevelItem( 1 );
    QTreeWidgetItem *sistema = ui->falhas->topLevelItem( 2 );
    QTreeWidgetItem *item = NULL;

    for ( int f = 0 ; f < lista_falhas.count() ; f++ )
    {
        abreviaturas << lista_falhas[f][1];
        descricoes << lista_falhas[f][2];

        if ( lista_falhas[f][0] == "Atuador" )
        {
            item = new QTreeWidgetItem( atuadores, 
                                        QStringList( abreviaturas[f] + 
                                                     " - " +
                                                     descricoes[f] ) );
        }
        else if ( lista_falhas[f][0] == "Sensor" )
        {
            item = new QTreeWidgetItem( sensores, 
                                        QStringList( abreviaturas[f] + 
                                                     " - " +
                                                     descricoes[f] ) );
        }
        else if ( lista_falhas[f][0] == "Sistema" )
        {
            item = new QTreeWidgetItem( sistema, 
                                        QStringList( abreviaturas[f] +
                                                     " - " +
                                                     descricoes[f] ) );
        }
            
        item->setToolTip( 0, descricoes[f] );
    }

    ui->falhas->expandAll();
}


void JanelaPrincipal :: atualizar_modulos( const QString &nome_arq )
{
    try
    {
        lista_modulos = manipulador_xml.ler_modulos( nome_arq );
        lista_modulos[0]->ler_arquivos();
    }
    catch( ExcecaoArquivo e )
    {
        throw e;
    }
    catch( ExcecaoLeituraXML e )
    {
        throw e;
    }
}


void JanelaPrincipal :: desabilitar_botoes_falhas()
{
    ui->botao_recarregar_falhas->setEnabled( false );
    ui->acao_cfg_falhas->setEnabled( false );

    ui->botao_carregar_modulos->setEnabled( false );
}


void JanelaPrincipal :: desabilitar_botoes_modulos()
{
    ui->botao_recarregar_modulos->setEnabled( false );
    ui->acao_cfg_modulos->setEnabled( false );
}


void JanelaPrincipal :: inicializar()
{
    cfg_falhas = NULL;
    cfg_modulos = NULL;
}


void JanelaPrincipal :: limpar_falhas()
{
    lista_falhas.clear();
    abreviaturas.clear();
    descricoes.clear();

    // Limpando os elementos da lista de falhas da interface
    QTreeWidgetItem *atuadores = ui->falhas->topLevelItem( 0 );
    QTreeWidgetItem *sensores = ui->falhas->topLevelItem( 1 );
    QTreeWidgetItem *sistema = ui->falhas->topLevelItem( 2 );
    
    Q_UNUSED( atuadores->takeChildren() );
    Q_UNUSED( sensores->takeChildren() );
    Q_UNUSED( sistema->takeChildren() );
}


void JanelaPrincipal :: limpar_modulos()
{
    lista_modulos.clear();

    ui->modulos->clearContents();
}


void JanelaPrincipal :: on_acao_cfg_falhas_triggered()
{
    if ( cfg_falhas != NULL )
        delete cfg_falhas;

    cfg_falhas = new ConfigFalhas( this, lista_falhas );

    cfg_falhas->exec();
}


void JanelaPrincipal :: on_acao_cfg_modulos_triggered()
{
    if ( cfg_modulos != NULL )
        delete cfg_modulos;

    cfg_modulos = new ConfigModulos( this, abreviaturas );

    cfg_modulos->exec();
}


void JanelaPrincipal :: on_botao_carregar_falhas_clicked()
{
    QString tipos_arq = "Aquivos Simddef (*.sdd)";

    QString nome_arq = QFileDialog::getOpenFileName( this, 
                                                     "Abrir arquivo...",
                                                     ".",
                                                     tipos_arq );

    if ( !nome_arq.isEmpty() )
    {
        limpar_falhas();

        try
        {
            atualizar_falhas( nome_arq );

            ui->end_arq_falhas->setText( nome_arq );

            ui->botao_recarregar_falhas->setEnabled( true );
            ui->acao_cfg_falhas->setEnabled( true );

            ui->botao_carregar_modulos->setEnabled( true );
        }
        catch( ExcecaoArquivo e )
        {
            exibir_mensagem( this, "Erro de leitura", e.msg_erro(), Aviso );
        }
        catch( ExcecaoLeituraXML e )
        {
            exibir_mensagem( this, "Erro de leitura", e.msg_erro(), Aviso );

            ui->end_arq_falhas->clear();

            desabilitar_botoes_falhas();

            // Se as falhas que estavam cadastradas foram "excluidas" apos o
            // erro, nao faz mais sentido manter os modulos cadastrados no
            // sistema
            limpar_modulos();

            desabilitar_botoes_modulos();
        }
    }
}


void JanelaPrincipal :: on_botao_carregar_modulos_clicked()
{
    QString tipos_arq = "Aquivos Simddef (*.sdd)";

    QString nome_arq = QFileDialog::getOpenFileName( this, 
                                                     "Abrir arquivo...",
                                                     ".",
                                                     tipos_arq );

    if ( !nome_arq.isEmpty() )
    {
        limpar_modulos();

        try
        {
            atualizar_modulos( nome_arq );

            ui->end_arq_modulos->setText( nome_arq );

            ui->botao_recarregar_modulos->setEnabled( true );
            ui->acao_cfg_modulos->setEnabled( true );
        }
        catch( ExcecaoLeituraXML e )
        {
            exibir_mensagem( this, "Erro de leitura", e.msg_erro(), Aviso );
            
            ui->end_arq_modulos->clear();

            desabilitar_botoes_modulos();
        }
    }
}


void JanelaPrincipal :: on_botao_recarregar_falhas_clicked()
{
    limpar_falhas();

    try
    {
        atualizar_falhas( ui->end_arq_falhas->text() );
    }
    catch( ExcecaoLeituraXML e )
    {
        exibir_mensagem( this, "Erro de leitura", e.msg_erro(), Aviso );

        ui->end_arq_falhas->clear();

        desabilitar_botoes_falhas();

        // Se as falhas que estavam cadastradas foram "excluidas" apos o erro,
        // nao faz mais sentido manter os modulos cadastrados no sistema
        limpar_modulos();

        desabilitar_botoes_modulos();
    }
}


void JanelaPrincipal :: on_botao_recarregar_modulos_clicked()
{
    limpar_modulos();

    try
    {
        atualizar_modulos( ui->end_arq_modulos->text() );
    }
    catch( ExcecaoLeituraXML e )
    {
        exibir_mensagem( this, "Erro de leitura", e.msg_erro(), Aviso );

        ui->end_arq_modulos->clear();

        desabilitar_botoes_modulos();
    }
}

#endif
