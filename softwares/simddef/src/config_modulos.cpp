#ifndef CONFIG_MODULOS_CPP_
#define CONFIG_MODULOS_CPP_

#include "config_modulos.h"

ConfigModulos :: ConfigModulos( QWidget *pai,
                                const QStringList &falhas ) :
                QDialog( pai )
{
    ui = new Ui_ConfigModulos;
    
    ui->setupUi( this );

    ui->falhas->insertItems( 0, falhas );
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

        for ( int a = 0 ; a < nomes_arqs.count() ; a++ )
        {
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
