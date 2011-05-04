#ifndef JANELA_INTERNA_CPP_
#define JANELA_INTERNA_CPP_

#include "janela_interna.h"

JanelaInterna :: JanelaInterna( QWidget *pai ) : QWidget( pai )
{
    ui = new Ui_JanelaInterna;

    ui->setupUi( this );
}

JanelaInterna :: ~JanelaInterna()
{
    delete ui;
}


void JanelaInterna :: cfg_dados( const QList< QList< double > > &dados )
{
}


void JanelaInterna :: exibir_saida()
{
}


void JanelaInterna :: exibir_deteccoes()
{
}

#endif
