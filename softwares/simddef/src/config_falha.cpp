#ifndef CONFIG_FALHA_CPP_
#define CONFIG_FALHA_CPP_

#include "config_falha.h"

ConfigFalha :: ConfigFalha()
{
    ui = new Ui_ConfigFalha;
    
    ui->setupUi( this );
}


ConfigFalha :: ~ConfigFalha()
{
    delete ui;
}


bool ConfigFalha :: carregar_falhas( const QString &nome_arq )
{
    QFile arq( nome_arq );

    QXmlInputSource *fonte = new QXmlInputSource( &arq );

    QXmlSimpleReader leitor;

    ManipuladorXml *manipulador_xml = new ManipuladorXml( ui->falhas_cad );

    leitor.setContentHandler( manipulador_xml );
    leitor.setErrorHandler( manipulador_xml );

    bool ok = leitor.parse( fonte );

    return true;
}

#endif
