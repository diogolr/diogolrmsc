#ifndef CONFIG_MODULO_CPP_
#define CONFIG_MODULO_CPP_

#include "config_modulo.h"

ConfigModulo :: ConfigModulo()
{
    ui = new Ui_ConfigModulo;
    
    ui->setupUi( this );
}


ConfigModulo :: ~ConfigModulo()
{
    delete ui;
}



QStringList ConfigModulo :: arquivos()
{
    qDebug() << nomes_arquivos;
    return nomes_arquivos;
}


void ConfigModulo :: on_botao_ok_clicked()
{
    // TODO capturar nomes dos arquivos de configuracao do modulo
    nomes_arquivos.append( "../cfgs_rnas/FADG/ent_rna_detec.dat" );
    nomes_arquivos.append( "../cfgs_rnas/FADG/O2N8T3_RNA.cfg" );
    nomes_arquivos.append( "../cfgs_rnas/FADG/O2N8T3_RNA.lim" );

    accept();
}


void ConfigModulo :: on_botao_cancel_clicked()
{
    reject();
}



#endif
