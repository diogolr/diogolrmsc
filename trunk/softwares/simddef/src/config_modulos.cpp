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

    carregar_modulos();
}


ConfigModulos :: ~ConfigModulos()
{
    delete ui;
}


void ConfigModulos :: atualizar_modulos()
{
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


void ConfigModulos :: on_adicionar_clicked()
{
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
            break;

        case Modulo::Estatistico:
            // TODO
            break;

        case Modulo::Personalizado:
        default:
            // TODO
            break;
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
}


void ConfigModulos :: on_subir_arq_clicked()
{
    int lin = ui->lista_arquivos->currentRow();

    ui->lista_arquivos->insertItem( lin, 
                                    ui->lista_arquivos->takeItem( lin - 1 ) );
}


#endif
