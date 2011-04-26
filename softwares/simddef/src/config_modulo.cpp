#ifndef CONFIG_MODULO_CPP_
#define CONFIG_MODULO_CPP_

#include "config_modulo.h"

ConfigModulo :: ConfigModulo( QWidget *pai, 
                              const QStringList &falhas ) : 
                QDialog( pai )
{
    ui = new Ui_ConfigModulo;
    
    ui->setupUi( this );

    ui->falhas->insertItems( 0, falhas );
}


ConfigModulo :: ~ConfigModulo()
{
    delete ui;
}


QStringList ConfigModulo :: arquivos()
{
    return nomes_arquivos;
}


void ConfigModulo :: on_botao_ok_clicked()
{
    // TODO capturar nomes dos arquivos de configuracao do modulo

    accept();
}

#endif
