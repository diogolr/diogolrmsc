#ifndef CONFIG_PARAMETROS_CPP_
#define CONFIG_PARAMETROS_CPP_

#include "config_parametros.h"

ConfigParametros :: ConfigParametros( QWidget *pai ) : QDialog( pai )
{
    ui = new Ui_ConfigParametros;

    ui->setupUi( this );

    this->setModal( true );
}


ConfigParametros :: ~ConfigParametros()
{
    delete ui;
}



QString ConfigParametros :: arquivo()
{
    return arq;
}


void ConfigParametros :: on_botao_arquivo_clicked()
{
    arq = QFileDialog::getOpenFileName( this, 
                                        "Abrir arquivo...", 
                                        ".", 
                                        utf8( "Arquivo de "
                                              "configuração (*.cfg)" ) );

    ui->arquivo->setText( arq );
}

#endif
