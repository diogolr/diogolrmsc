#ifndef CONFIG_FALHA_CPP_
#define CONFIG_FALHA_CPP_

#include "config_falha.h"

ConfigFalha :: ConfigFalha( const QString &arq,
                            QWidget *pai ) : QDialog( pai )
{
    ui = new Ui_ConfigFalha;
    
    ui->setupUi( this );

    nome_arq = arq;
}


ConfigFalha :: ~ConfigFalha()
{
    delete ui;
}


QList< QStringList > ConfigFalha :: ler_falhas()
{
    inicializar();
    carregar_falhas();
    ler_tabela();
    return falhas;
}


void ConfigFalha :: carregar_falhas()
{
    // Limpando as falhas ja existentes na tabela para recarregar os arquivos
    ui->falhas_cad->clearContents();

    // Leitura do arquivo XML de configuracao das falhas
    QFile arq( nome_arq );

    if ( !arq.open( QIODevice::ReadOnly | QIODevice::Text ) )
    {
        throw ExcecaoArquivo( "O arquivo não pôde ser aberto para leitura." );
    }

    QXmlInputSource *fonte = new QXmlInputSource( &arq );

    QXmlSimpleReader leitor;

    ManipuladorXml *manipulador_xml = new ManipuladorXml( ui->falhas_cad );

    leitor.setContentHandler( manipulador_xml );
    leitor.setErrorHandler( manipulador_xml );

    leitor.parse( fonte );

    arq.close();

    // Redimensionando as colunas de acordo com o conteudo
    for ( int i = 0 ; i < ui->falhas_cad->columnCount() ; i++ )
    {
        ui->falhas_cad->resizeColumnToContents( i );
    }
}


void ConfigFalha :: configurar_arquivo( const QString &arq )
{
    nome_arq = arq;
}


void ConfigFalha :: inicializar()
{
    ui->salvar->setEnabled( false );
}


void ConfigFalha :: ler_tabela()
{
    falhas.clear();

    // Adicionando as falhas na lista de falhas
    for ( int i = 0 ; i < ui->falhas_cad->rowCount() ; i++ )
    {
        QStringList lista;

        lista << ui->falhas_cad->item( i, 0 )->text()
              << ui->falhas_cad->item( i, 1 )->text()
              << ui->falhas_cad->item( i, 2 )->text();

        falhas.append( lista );
    }
}


void ConfigFalha :: on_adicionar_clicked()
{
    ui->salvar->setEnabled( true );

    QString loc, abv, des;

    // Local da falha
    switch( ui->local->currentIndex() )
    {
        // Falha do atuador
        case 0:
            loc = "Atuador";
            break;

        // Falha do sensor
        case 1:
            loc = "Sensor";
            break;

        // Falha do sistema
        case 2:
            loc = "Sistema";
            break;

        default:
            // Nunca deve acontecer
            break;
    }

    // Abreviatura e descricao
    abv = ui->abrv->text();
    des = ui->descricao->text();

    if ( abv.isEmpty() )
    {
        exibir_mensagem( this, "Erro", "Abreviatura inválida.", Aviso );
        return;
    }

    if ( des.isEmpty() )
    {
        exibir_mensagem( this, "Erro", "Descrição inválida.", Aviso );
        return;
    }

    // Verificando se ja existe um item com a mesma abreviacao ou descricao
    if ( ui->falhas_cad->findItems( abv, Qt::MatchExactly ).count() != 0 ||
         ui->falhas_cad->findItems( des, Qt::MatchExactly ).count() != 0 )
    {
        QMessageBox msg( this );

        msg.setModal( true );
        msg.setIcon( QMessageBox::Question );
        msg.setText( utf8( "Abreviatura ou descrição já existe.") );
        msg.setInformativeText( "Deseja adicionar mesmo assim?" );
        
        QPushButton *sim = msg.addButton( "Sim", QMessageBox::YesRole );
        QPushButton *nao = msg.addButton( utf8( "Não" ), QMessageBox::NoRole );
        
        msg.setDefaultButton( nao );

        msg.exec();

        Q_UNUSED( sim );

        if ( msg.clickedButton() == nao )
            return;
    }

    // Inserindo a nova linha
    int lin = ui->falhas_cad->rowCount();

    ui->falhas_cad->insertRow( lin );

    // Desabilitando a ordenacao para inserir o item corretamente e so depois
    // ordenar
    ui->falhas_cad->setSortingEnabled( false );

    QTableWidgetItem *local = new QTableWidgetItem( loc );
    QTableWidgetItem *abrv = new QTableWidgetItem( abv );
    QTableWidgetItem *desc = new QTableWidgetItem( des );

    local->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    abrv->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    ui->falhas_cad->setItem( lin, 0, local );
    ui->falhas_cad->setItem( lin, 1, abrv );
    ui->falhas_cad->setItem( lin, 2, desc );

    ui->falhas_cad->sortItems( 1, Qt::AscendingOrder );
    
    // Reabilitando a ordenacao
    ui->falhas_cad->setSortingEnabled( true );
}


void ConfigFalha :: on_remover_clicked()
{
    ui->salvar->setEnabled( true );

    QTableWidgetItem *item = ui->falhas_cad->currentItem();

    if ( item == 0 )
    {
        exibir_mensagem( this, "Erro", "Nenhum item foi selecionado", Aviso );
        return;
    }

    QMessageBox msg( this );

    msg.setModal( true );
    msg.setIcon( QMessageBox::Question );
    msg.setText( "Deseja realmente remover o item selecionado?" );
    
    QPushButton *sim = msg.addButton( "Sim", QMessageBox::YesRole );
    QPushButton *nao = msg.addButton( utf8( "Não" ), QMessageBox::NoRole );
        
    msg.setDefaultButton( nao );

    msg.exec();

    Q_UNUSED( sim );

    if ( msg.clickedButton() == nao )
        return;

    ui->falhas_cad->removeRow( item->row() );
}


void ConfigFalha :: on_salvar_clicked()
{
    QFile arq( nome_arq );

    if ( !arq.open( QIODevice::WriteOnly ) )
    {
        exibir_mensagem( this, 
                         "Erro", 
                         "O arquivo não pôde ser aberto para escrita.",
                         Aviso );
    }

    // Leitura da tabela atual que esta sendo exibida para salvar o arquivo
    // corretamente
    ler_tabela();

    // Escrita no arquivo
    QXmlStreamWriter stream( &arq );

    stream.setAutoFormatting( true );
    stream.writeStartDocument();
    stream.writeStartElement( "Simddef" );
    stream.writeAttribute( "version", "1.0" );
    stream.writeAttribute( "tipo", "falhas" );

    for ( int f = 0 ; f < falhas.count() ; f++ )
    {
        stream.writeStartElement( "Falha" );
        stream.writeAttribute( "local", falhas[f][0] );
        stream.writeAttribute( "abrv", falhas[f][1] );
        stream.writeAttribute( "descricao", falhas[f][2] );
        stream.writeEndElement();
    }

    stream.writeEndElement();
    stream.writeEndDocument();

    arq.close();

    exibir_mensagem( this, 
                     "Arquivo salvo com sucesso", 
                     "O arquivo foi salvo com sucesso.",
                     Informacao );

    ui->salvar->setEnabled( false );
}

#endif
