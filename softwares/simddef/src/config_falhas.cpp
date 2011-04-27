#ifndef CONFIG_FALHAS_CPP_
#define CONFIG_FALHAS_CPP_

#include "config_falhas.h"

ConfigFalhas :: ConfigFalhas( QWidget *pai, 
                              const QList< QStringList > &lista_falhas ) :
                QDialog( pai ), 
                falhas( lista_falhas )
{
    ui = new Ui_ConfigFalhas;
    
    ui->setupUi( this );
    
    carregar_falhas();
}


ConfigFalhas :: ~ConfigFalhas()
{
    delete ui;
}


void ConfigFalhas :: atualizar_lista()
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


void ConfigFalhas :: carregar_falhas()
{
    // Limpando as falhas ja existentes na tabela para recarregar os arquivos
    ui->falhas_cad->clearContents();

    // Desabilitando a ordenacao para inserir o item corretamente e so
    // depois ordenar
    ui->falhas_cad->setSortingEnabled( false );

    for ( int f = 0 ; f < falhas.count() ; f++ )
    {
        // Elementos serao sempre inseridos na linha 0
        ui->falhas_cad->insertRow( 0 );

        QTableWidgetItem *local = new QTableWidgetItem( falhas[f][0] );
        QTableWidgetItem *abrv = new QTableWidgetItem( falhas[f][1] );
        QTableWidgetItem *desc = new QTableWidgetItem( falhas[f][2] );

        local->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
        abrv->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

        // Elementos a serem modificados sempre sao aqueles inseridos na linha 0
        ui->falhas_cad->setItem( 0, 0, local );
        ui->falhas_cad->setItem( 0, 1, abrv );
        ui->falhas_cad->setItem( 0, 2, desc );
    }

    // Ordenacao dos elementos de acordo com a coluna 1 (abreviatura) depois que
    // todos foram inseridos
    ui->falhas_cad->sortItems( 1, Qt::AscendingOrder );

    // Reabilitando a ordenacao para o usuario
    ui->falhas_cad->setSortingEnabled( true );

    // Redimensionando as colunas de acordo com o conteudo
    for ( int i = 0 ; i < ui->falhas_cad->columnCount() ; i++ )
    {
        ui->falhas_cad->resizeColumnToContents( i );
    }
}


void ConfigFalhas :: on_adicionar_clicked()
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


void ConfigFalhas :: on_remover_clicked()
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


void ConfigFalhas :: on_salvar_clicked()
{
    QString tipos_arq = "Arquivos Simddef (*.sdd)";

    QString nome_arq = QFileDialog::getSaveFileName( this, 
                                                     "Salvar como...",
                                                     ".",
                                                     tipos_arq );

    if ( !nome_arq.isEmpty() )
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
        atualizar_lista();

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
}

#endif
