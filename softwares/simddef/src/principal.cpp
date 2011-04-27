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


void JanelaPrincipal :: atualizar_falhas( const QString &nome_arq )
{
    // Leitura do arquivo XML de configuracao das falhas
    QFile arq( nome_arq );

    if ( !arq.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        throw ExcecaoArquivo( "O arquivo não pôde ser aberto para leitura." );
    }

    QXmlInputSource *fonte = new QXmlInputSource( &arq );

    QXmlSimpleReader leitor;

    ManipuladorXml *manipulador_xml = new ManipuladorXml( &lista_falhas );

    leitor.setContentHandler( manipulador_xml );
    leitor.setErrorHandler( manipulador_xml );

    try
    {
        leitor.parse( fonte );
    }
    catch( ExcecaoLeituraXML e )
    {
        throw e;
    }

    arq.close();

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
        else
        {
            throw ExcecaoLeituraXML( "Local da falha inválido" );
        }
            
        item->setToolTip( 0, descricoes[f] );
    }

    ui->falhas->expandAll();
}


void JanelaPrincipal :: inicializar()
{
    cfg_falhas = NULL;
    cfg_modulo = NULL;
    
    limpar_falhas();
}


void JanelaPrincipal :: limpar_falhas()
{
    lista_falhas.clear();
    abreviaturas.clear();
    descricoes.clear();
    ui->end_arq_falhas->clear();
    
    ui->botao_adicionar_modulo->setEnabled( false );
    ui->botao_recarregar_falhas->setEnabled( false );
    ui->acao_cfg_falhas->setEnabled( false );
}


void JanelaPrincipal :: on_acao_cfg_falhas_triggered()
{
    if ( cfg_falhas != NULL )
        delete cfg_falhas;

    cfg_falhas = new ConfigFalhas( this, lista_falhas );

    cfg_falhas->exec();
}


void JanelaPrincipal :: on_botao_adicionar_modulo_clicked()
{
    if ( cfg_modulo != NULL )
        delete cfg_modulo;

    cfg_modulo = new ConfigModulo( abreviaturas, this );

    // Saindo se cancelar for pressionado ou continuando se o modulo foi
    // configurado corretamente
    switch( cfg_modulo->exec() )
    {
        case QDialog::Rejected:
            return;
    }

    // A configuracao dos arquivos necessarios varia de acordo com cada modulo.
    QStringList arquivos;

    // Criando um ponteiro para cada um dos tipos de modulo para evitar
    // problemas de alocacao dinamica dentro do switch
    Rede *rede;

    // Selecionando o tipo de modulo a ser adicionado
    switch( cfg_modulo->tipo() )
    {
        // Configuracao de um modulo neural
        case Modulo::RNA:
            rede = new Rede;

            modulos << rede;
                    
            arquivos << cfg_modulo->arquivos();

            rede->ler_arquivos( arquivos );

            // TODO adicionar modulo na tabela da interface
            
            break;

        // Configuracao de um modulo fuzzy
        case Modulo::Fuzzy:
            break;

        // Configuracao de um modulo estatistico
        case Modulo::Estatistico:
            break;

        // Configuracao de um modulo personalizado
        case Modulo::Personalizado:
        default:
            break;
    }
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
        try
        {
            atualizar_falhas( nome_arq );

            ui->end_arq_falhas->setText( nome_arq );

            ui->botao_adicionar_modulo->setEnabled( true );
            ui->botao_recarregar_falhas->setEnabled( true );
            ui->acao_cfg_falhas->setEnabled( true );
        }
        catch( ExcecaoLeituraXML e )
        {
            exibir_mensagem( this, "Erro de leitura", e.msg_erro(), Aviso );

            limpar_falhas();
        }
    }
}


void JanelaPrincipal :: on_botao_carregar_modulos_clicked()
{
}


void JanelaPrincipal :: on_botao_recarregar_falhas_clicked()
{
    try
    {
        atualizar_falhas( ui->end_arq_falhas->text() );

        ui->botao_adicionar_modulo->setEnabled( true );
        ui->botao_recarregar_falhas->setEnabled( true );
        ui->acao_cfg_falhas->setEnabled( true );
    }
    catch( ExcecaoLeituraXML e )
    {
        exibir_mensagem( this, "Erro de leitura", e.msg_erro(), Aviso );

        limpar_falhas();
    }
}

#endif
