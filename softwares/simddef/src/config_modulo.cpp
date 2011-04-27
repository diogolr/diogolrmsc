#ifndef CONFIG_MODULO_CPP_
#define CONFIG_MODULO_CPP_

#include "config_modulo.h"

ConfigModulo :: ConfigModulo( const QStringList &falhas,
                              Modulo *mod,
                              QWidget *pai ) :
                QDialog( pai )
{
    ui = new Ui_ConfigModulo;
    
    ui->setupUi( this );

    ui->falhas->insertItems( 0, falhas );

    ui->tipo->setCurrentIndex( mod->tipo() );

    nomes_arquivos.clear();
}


ConfigModulo :: ConfigModulo( const QStringList &falhas,
                              QWidget *pai ) : 
                QDialog( pai )
{
    ui = new Ui_ConfigModulo;
    
    ui->setupUi( this );

    ui->falhas->insertItems( 0, falhas );

    nomes_arquivos.clear();
}


ConfigModulo :: ~ConfigModulo()
{
    delete ui;
}


Modulo::TipoModulo ConfigModulo :: tipo()
{
    return (Modulo::TipoModulo)ui->tipo->currentIndex();
}


QString ConfigModulo :: falha()
{
    return ui->falhas->currentText();
}


QStringList ConfigModulo :: arquivos()
{
    return nomes_arquivos;
}


void ConfigModulo :: on_adicionar_clicked()
{
    QString tipos_arq;

    switch( ui->tipo->currentIndex() )
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

                nomes_arquivos << nomes_arqs[a];
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
                             "não foram adicionados novamente.",
                             Informacao );
        }
    }
}


void ConfigModulo :: on_descer_clicked()
{
    int lin = ui->lista_arquivos->currentRow();

    ui->lista_arquivos->insertItem( lin, 
                                    ui->lista_arquivos->takeItem( lin + 1 ) );
}


void ConfigModulo :: on_remover_clicked()
{
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

    int lin = ui->lista_arquivos->currentRow();

    QListWidgetItem *item = ui->lista_arquivos->takeItem( lin ); 

    nomes_arquivos.removeAll( item->text() );
}


void ConfigModulo :: on_subir_clicked()
{
    int lin = ui->lista_arquivos->currentRow();

    ui->lista_arquivos->insertItem( lin, 
                                    ui->lista_arquivos->takeItem( lin - 1 ) );
}


#endif
