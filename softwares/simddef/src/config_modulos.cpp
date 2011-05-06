#ifndef CONFIG_MODULOS_CPP_
#define CONFIG_MODULOS_CPP_

#include "config_modulos.h"

ConfigModulos :: ConfigModulos( QWidget *pai,
                                const QStringList &falhas,
                                const QList< Modulo * > &mods ) :
                 QDialog( pai )
{
    ui = new Ui_ConfigModulos;
    
    ui->setupUi( this );

    ui->falhas->insertItems( 0, falhas );

    modulos = mods;

    inicializar();

    carregar_modulos();
}


ConfigModulos :: ~ConfigModulos()
{
    delete ui;
}


void ConfigModulos :: carregar_modulos()
{
    // Limpando os modulos que ja existem
    ui->modulos_cad->clearContents();

    // Desabilitando a ordenacao para inserir os itens corretamente e so depois
    // ordena-los
    ui->modulos_cad->setSortingEnabled( false );

    QStringList arqs;

    Modulo *modulo;

    // Inserindo os modulos na lista
    for ( int m = 0 ; m < modulos.count() ; m++ )
    {
        ui->modulos_cad->insertRow( 0 );

        modulo = modulos[m];

        // Itens a serem inseridos
        QTableWidgetItem *tipo = new QTableWidgetItem( modulo->nome_tipo() );
        QTableWidgetItem *falha = new QTableWidgetItem( modulo->nome_falha() );

        arqs = modulo->endereco_arquivos();

        QTableWidgetItem *qtde = new QTableWidgetItem( numero( arqs.count() ) );
        
        // Alinhamento dos textos nas celulas
        tipo->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
        falha->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
        qtde->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

        // Insercao dos itens (sempre na linha zero)
        ui->modulos_cad->setItem( 0, 0, tipo );
        ui->modulos_cad->setItem( 0, 1, falha );
        ui->modulos_cad->setItem( 0, 2, qtde );

        // Fazendo o mapeamento de um dos itens da linha (qualquer um dos tres
        // teria a mesma funcao no mapeamento) para o respectivo modulo. Isso
        // permitira fazer alteracoes nos modulos posteriormente
        mapeamento_modulos[ tipo ] = modulo;
    }

    // Reabilitando a ordenacao
    ui->modulos_cad->setSortingEnabled( true );

    // Ordenando os itens segundo o nome das falhas (coluna 2)
    ui->modulos_cad->sortItems( 1, Qt::AscendingOrder );

    // Ajustando a largura das colunas de acordo com o conteudo das celulas
    ui->modulos_cad->resizeColumnsToContents();
}


void ConfigModulos :: desabilitar_ordem()
{
    ui->label_ordem->setVisible( false );
    ui->ordem->setVisible( false );
}


void ConfigModulos :: habilitar_ordem()
{
    ui->label_ordem->setVisible( true );
    ui->ordem->setVisible( true );
}


void ConfigModulos :: inicializar()
{
    if ( ui->tipos->currentText() != "RNA" )
    {
        ui->label_ordem->setVisible( false );
        ui->ordem->setVisible( false );
    }
}


void ConfigModulos :: on_adicionar_clicked()
{
    // Lista de arquivos
    QStringList arqs;

    int num_arqs = ui->lista_arquivos->count();

    if ( num_arqs == 0 )
    {
        exibir_mensagem( this, 
                         "Erro", 
                         "Nenhum arquivo na lista de arquivos de configuração "
                         "do módulo.",
                         Aviso );
        return;
    }

    for ( int i = 0 ; i < num_arqs ; i++ )
    {
        arqs << ui->lista_arquivos->item( i )->text();
    }

    // Criando o novo modulo
    Modulo *modulo;

    bool ok = true; 
    int ordem = -1;

    switch( ui->tipos->currentIndex() )
    {
        case Modulo::RNA:
            modulo = new Rede;

            ordem =  ui->ordem->text().toInt( &ok );

            if ( !ok )
            {
                exibir_mensagem( this, 
                                 "Erro", 
                                 "Ordem inválida.", 
                                 Aviso );
                return;
            }

            ((Rede *)modulo)->configurar_ordem( ordem );

            break;

        case Modulo::Fuzzy:
            // TODO
            exibir_mensagem( this, 
                             "Erro", 
                             "Módulo <b>Fuzzy</b> não implementado.", 
                             Aviso );
            return;
            //break;

        case Modulo::Estatistico:
            // TODO
            exibir_mensagem( this, 
                             "Erro", 
                             "Módulo <b>Estatístico</b> não implementado.", 
                             Aviso );
            return;
            //break;

        case Modulo::Personalizado:
        default:
            // TODO
            exibir_mensagem( this, 
                             "Erro", 
                             "Módulo <b>Personalizado</b> não implementado.", 
                             Aviso );
            return;
            //break;
    }

    // Configurando os parametros do modulo
    modulo->configurar_arquivos( arqs );
    modulo->configurar_falha( ui->falhas->currentText() );

    // Adicionando o modulo a lista de modulos
    modulos << modulo;

    // Adicionando o modulo no lista de modulos cadastrados da interface
    ui->modulos_cad->setSortingEnabled( false );

    ui->modulos_cad->insertRow( 0 );

    QTableWidgetItem *tipo = new QTableWidgetItem( modulo->nome_tipo() );
    QTableWidgetItem *falha = new QTableWidgetItem( modulo->nome_falha() );
    QTableWidgetItem *qtde = new QTableWidgetItem( numero( arqs.count() ) );
    
    // Alinhamento dos textos nas celulas
    tipo->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    falha->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );
    qtde->setTextAlignment( Qt::AlignHCenter | Qt::AlignVCenter );

    // Insercao dos itens (sempre na linha zero)
    ui->modulos_cad->setItem( 0, 0, tipo );
    ui->modulos_cad->setItem( 0, 1, falha );
    ui->modulos_cad->setItem( 0, 2, qtde );

    // Fazendo o mapeamento de um dos itens da linha (qualquer um dos tres
    // teria a mesma funcao no mapeamento) para o respectivo modulo. Isso
    // permitira fazer alteracoes nos modulos posteriormente
    mapeamento_modulos[ tipo ] = modulo;

    // Reabilitando a ordenacao
    ui->modulos_cad->setSortingEnabled( true );

    // Ordenando os itens
    ui->modulos_cad->sortItems( 1, Qt::AscendingOrder );

    // Ajustando a largura das colunas de acordo com o conteudo das celulas
    ui->modulos_cad->resizeColumnsToContents();

    // Limpando os campos da interface para que um novo modulo possa ser
    // inserido
    ui->lista_arquivos->clear();
    ui->falhas->setCurrentIndex( 0 );
    ui->tipos->setCurrentIndex( 0 );
}


void ConfigModulos :: on_adicionar_arq_clicked()
{
    QString tipos_arq;

    switch( ui->tipos->currentIndex() )
    {
        case Modulo::RNA:
            tipos_arq = "Arquivos de RNAs (*.cfg *.net *.dat *.lim)";
            break;

        case Modulo::Fuzzy:
            // TODO
            exibir_mensagem( this, 
                             "Erro", 
                             "Módulo <b>Fuzzy</b> não implementado.", 
                             Aviso );
            return;
            //break;

        case Modulo::Estatistico:
            // TODO
            exibir_mensagem( this, 
                             "Erro", 
                             "Módulo <b>Estatístico</b> não implementado.", 
                             Aviso );
            return;
            //break;

        case Modulo::Personalizado:
        default:
            // TODO
            exibir_mensagem( this, 
                             "Erro", 
                             "Módulo <b>Personalizado</b> não implementado.", 
                             Aviso );
            return;
            //break;
    }

    // Capturando o(s) nome(s) do(s) arquivo(s)
    QStringList nomes_arqs = QFileDialog::getOpenFileNames( this,
                                                          "Abrir arquivo(s)...",
                                                            ".",
                                                            tipos_arq );

    if ( nomes_arqs.count() != 0 )
    {
        bool flag = false;

        QDir dir;

        for ( int a = 0 ; a < nomes_arqs.count() ; a++ )
        {
            // Transformando os nomes dos caminhos absolutos em nomes de
            // caminhos relativos aos executaveis
            nomes_arqs[a] = dir.relativeFilePath( nomes_arqs[a] );

            // Adiciona se o arquivo ja nao estiver na lista
            if ( !ui->lista_arquivos->findItems( nomes_arqs[a], 
                                                 Qt::MatchExactly ).count() )
            {
                ui->lista_arquivos->addItem( nomes_arqs[a] );
            }
            else
            {
                flag = true;
            }
        }

        if ( flag )
        {
            exibir_mensagem( this, 
                             "Aviso", 
                             "Alguns arquivos já estavam na lista e, portanto, "
                             "não foram novamente adicionados.",
                             Informacao );
        }

        ui->salvar->setEnabled( true );
    }
}


void ConfigModulos :: on_descer_arq_clicked()
{
    int lin = ui->lista_arquivos->currentRow();

    ui->lista_arquivos->insertItem( lin, 
                                    ui->lista_arquivos->takeItem( lin + 1 ) );
}


void ConfigModulos :: on_modulos_cad_itemDoubleClicked( QTableWidgetItem *it )
{
    // Limpando a lista de arquivos
    ui->lista_arquivos->clear();

    // Como o mapeamento foi realizado a partir da coluna tipo, sera obtido o
    // item desta celula (coluna 1) de acordo com a linha do item clicado
    QTableWidgetItem *item = ui->modulos_cad->item( it->row(), 0 );

    // Obtendo o modulo correspondente a celula que foi clicada
    Modulo *modulo = mapeamento_modulos[ item ];

    // Exibindo os arquivos carregados para aquele modulo
    QStringList arquivos = modulo->endereco_arquivos();

    for ( int a = 0 ; a < arquivos.count() ; a++ )
    {
        ui->lista_arquivos->addItem( arquivos[a] );
    }

    // Configurando o tipo do modulo e a falha a ele associada
    int indice_falha = ui->falhas->findText( modulo->nome_falha() );
    int indice_tipo = ui->tipos->findText( modulo->nome_tipo() );

    ui->falhas->setCurrentIndex( indice_falha );
    ui->tipos->setCurrentIndex( indice_tipo );
}


void ConfigModulos :: on_remover_clicked()
{
    QTableWidgetItem *it = ui->modulos_cad->currentItem();

    if ( it == NULL )
    {
        exibir_mensagem( this,
                         "Erro",
                         "Nenhum item selecionado para a remoção.",
                         Aviso );
        return;
    }

    // Verificando se o usuario deseja realmente remover o item selecionado
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

    // Linha do item selecionado
    int lin = it->row();

    // Item utilizado no mapeamento do modulo
    QTableWidgetItem *item = ui->modulos_cad->item( lin, 0 );

    // Modo correspondente a linha selecionada
    Modulo *modulo = mapeamento_modulos[ item ];

    // Removendo o modulo
    modulos.removeAll( modulo );

    // Removendo o mapeamento
    mapeamento_modulos.remove( item );

    // Removendo a linha da lista de modulos cadastrados
    ui->modulos_cad->removeRow( lin );

    // Habilitando o botao salvar
    ui->salvar->setEnabled( true );
}


void ConfigModulos :: on_remover_arq_clicked()
{
    int lin = ui->lista_arquivos->currentRow();

    if ( lin == -1 )
    {
        exibir_mensagem( this, "Erro", "Nenhum item selecionado", Aviso );
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

    QListWidgetItem *item = ui->lista_arquivos->takeItem( lin ); 

    Q_UNUSED( item );

    ui->salvar->setEnabled( true );
}


void ConfigModulos :: on_salvar_clicked()
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

        // Escrita no arquivo
        QXmlStreamWriter stream( &arq );

        stream.setAutoFormatting( true );
        stream.writeStartDocument();
        stream.writeStartElement( "Simddef" );        // <Simddef
        stream.writeAttribute( "versao", "1.0" );     // versao="1.0"
        stream.writeAttribute( "tipo", "modulos" );   // tipo="falhas">
        
        QString nome_tipo; 
        QString nome_falha;

        for ( int m = 0 ; m < modulos.count() ; m++ )
        {
            // Nome do tipo de modulo e da falha
            nome_tipo = modulos[m]->nome_tipo();
            nome_falha = modulos[m]->nome_falha();

            stream.writeStartElement( "Modulo" );               // <Modulo>
            stream.writeStartElement( "tipo" );                 // <tipo
            stream.writeAttribute( "nome", nome_tipo );         // nome="..."

            if ( nome_tipo == "RNA" )
            {
                Rede *rede = (Rede *)modulos[m];

                stream.writeStartElement( "ordem" );               // <ordem>
                stream.writeCharacters( numero( rede->ordem() ) ); // ...
                stream.writeEndElement();                          // </ordem>
            }
            // TODO
            /*
            else if ( nome_tipo == "Fuzzy" )
            {
            }
            else if ( nome_tipo == "Estatistico" )
            {
            }
            else if ( nome_tipo == "Personalizado" )
            {
            }
            */
            
            stream.writeEndElement();                           // </tipo>

            stream.writeStartElement( "falha" );                // <falha>
            stream.writeCharacters( nome_falha );               // ...
            stream.writeEndElement();                           // </falha>

            QStringList arqs = modulos[m]->endereco_arquivos();

            stream.writeStartElement( "arquivos" );             // <arquivos
            stream.writeAttribute( "qtde", 
                                   numero( arqs.count() ) );    // qtde="...">

            for ( int a = 0 ; a < arqs.count() ; a++ )
            {
                stream.writeStartElement( "arq" );              // <arq
                stream.writeAttribute( "end", arqs[a] );        // end="..."
                stream.writeEndElement();                       // />
            }

            stream.writeEndElement();                           // </arquivos>
            stream.writeEndElement();                           // </Modulo>
        }

        stream.writeEndElement();                    // </Simddef>
        stream.writeEndDocument();

        arq.close();

        exibir_mensagem( this, 
                         "Arquivo salvo com sucesso", 
                         "O arquivo foi salvo com sucesso.",
                         Informacao );

        ui->salvar->setEnabled( false );
    }
}


void ConfigModulos :: on_subir_arq_clicked()
{
    int lin = ui->lista_arquivos->currentRow();

    ui->lista_arquivos->insertItem( lin, 
                                    ui->lista_arquivos->takeItem( lin - 1 ) );
}


void ConfigModulos :: on_tipos_textChanged( QString str )
{
    if ( str == "RNA" )
    {
        habilitar_ordem();
    }
    else
    {
        desabilitar_ordem();
    }
}


#endif
