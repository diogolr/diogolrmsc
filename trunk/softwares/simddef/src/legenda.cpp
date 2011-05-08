#ifndef LEGENDA_CPP_
#define LEGENDA_CPP_

#include "legenda.h"

Legenda :: Legenda( QWidget *pai ) : QWidget( pai )
{
    ui = new Ui_Legenda;

    ui->setupUi( this );

    inicializar();
}


Legenda :: ~Legenda()
{
    delete ui;
}


void Legenda :: inicializar()
{
    // Retirando a borda
    ui->scroll->setFrameStyle( QFrame::NoFrame );
}

#endif
