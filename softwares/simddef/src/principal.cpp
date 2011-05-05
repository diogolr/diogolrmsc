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


void JanelaPrincipal :: ativar_modulo( Modulo *modulo )
{
    JanelaInterna *janela = new JanelaInterna( ui->mdi );

    ui->mdi->addSubWindow( janela );

    // TODO Simular sistema (a partir da rede identificada ou de um arquivo de
    // configuracao para execucao do script do matlab)
    // TODO Processar dados da simulacao e gerar matriz de entrada do modulo
    // neural (olhar metodo ler_arquivos da classe Rede)

    // Obtendo a saida do modulo
    MatrizD matriz = modulo->executar();

    /*
    // Processando os dados de saida
    QList< QList< double > > valores = processar_dados( matriz );

    // Configurando o grafico com os valores obtidos a partir do resultado do
    // processamento
    janela->cfg_dados( valores );

    // Exibir saida
    janela->exibir_saida();

    // Exibindo as deteccoes de falha
    janela->exibir_deteccoes();
    */

    // Exibindo a janela
    janela->show();
}


void JanelaPrincipal :: atualizar_falhas( const QString &nome_arq )
{
    try
    {
        lista_falhas = manipulador_xml.ler_falhas( nome_arq );
    }
    catch( Excecao e )
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
    }
    catch( Excecao e )
    {
        throw e;
    }

    int num_modulos = lista_modulos.count();

    QTableWidgetItem *item;

    QString nome_falha;

    // Desabilitando a ordenacao para que a insercao seja feita de maneira
    // correta
    ui->modulos->setSortingEnabled( false );

    // Barra de progresso
    QProgressDialog progresso( utf8( "Carregando os módulos..." ), 
                               "Cancelar", 
                               0,           // minimo 
                               num_modulos, // maximo
                               this );

    progresso.setWindowModality( Qt::WindowModal );

    for ( int m = 0 ; m < num_modulos ; m++ )
    {
        nome_falha = lista_modulos[m]->nome_falha();

        progresso.setValue( m );
     
        if ( progresso.wasCanceled() )
        {
            throw Excecao( "Processo de carregamento dos módulos "
                           "cancelado pelo usuário" );
        }

        // Conferindo se os modulos estao associados a falhas carregadas
        if ( !abreviaturas.contains( nome_falha ) )
        {
            progresso.close();

            throw ExcecaoAssociacao( "Erro de associação de falha ao "
                                     "tentar carregar o módulo. Falha <b>" +
                                     nome_falha +
                                     "</b> ainda não cadastrada." );
        }

        // Leitura dos arquivos de configuracao dos modulos
        lista_modulos[m]->ler_arquivos();

        // Exibicao dos modulos na janela principal ----------------------------
        // A tabela da interface possui 3 campos a serem preenchidos (tipo
        // do modulo, nome da falha a qual o modulo esta associado e o
        // status do modulo)
        ui->modulos->insertRow( 0 );
        
        // Tipo ----------------------------------------------------------------
        item = new QTableWidgetItem( lista_modulos[m]->nome_tipo() );

        item->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

        ui->modulos->setItem( 0, 0, item );

        // Falha ---------------------------------------------------------------
        item = new QTableWidgetItem( nome_falha );

        item->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

        ui->modulos->setItem( 0, 1, item );

        // Status --------------------------------------------------------------
        item = new QTableWidgetItem( "Desativado" );

        item->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

        // O mapeamento so e feito para o terceiro item porque e ele que contem
        // o status (ativado/desativado) do modulo
        mapeamento_modulos[item] = lista_modulos[m];

        ui->modulos->setItem( 0, 2, item );
    }
        
    // Reabilitando a ordenacao e ordenando os modulos de acordo com o nome
    // das falhas
    ui->modulos->setSortingEnabled( true );
    ui->modulos->sortByColumn( 1, Qt::AscendingOrder );

    // Redimensionando as colunas de acordo com o conteudo das celulas
    ui->modulos->resizeColumnsToContents();
        
    progresso.setValue( num_modulos );
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


void JanelaPrincipal :: desativar_modulo( Modulo * )
{
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
    mapeamento_modulos.clear();

    ui->modulos->clearContents();
    ui->modulos->setRowCount( 0 );
}


void JanelaPrincipal :: processar_dados()
{

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

    cfg_modulos = new ConfigModulos( this, abreviaturas, lista_modulos );

    cfg_modulos->exec();
}


void JanelaPrincipal :: on_acao_sair_triggered()
{
    this->close();
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
        catch( Excecao e )
        {
            exibir_mensagem( this, "Erro", e.msg_erro(), Aviso );

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
        catch( Excecao e )
        {
            exibir_mensagem( this, "Erro", e.msg_erro(), Aviso );
            
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
    catch( Excecao e )
    {
        exibir_mensagem( this, "Erro", e.msg_erro(), Aviso );

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
    catch( Excecao e )
    {
        exibir_mensagem( this, "Erro", e.msg_erro(), Aviso );

        ui->end_arq_modulos->clear();

        desabilitar_botoes_modulos();
    }
}


void JanelaPrincipal :: on_modulos_itemDoubleClicked( QTableWidgetItem *it )
{
    // O item que contem de status (ativado ou desativado) fica na terceira
    // coluna da tabela
    QTableWidgetItem *item = ui->modulos->item( it->row(), 2 );

    if ( item->text() == "Desativado" )
    {
        ativar_modulo( mapeamento_modulos[ item ] );
        item->setText( "Ativado" );
    }
    else
    {
        desativar_modulo( mapeamento_modulos[ item ] );
        item->setText( "Desativado" );
    }
}

#endif
