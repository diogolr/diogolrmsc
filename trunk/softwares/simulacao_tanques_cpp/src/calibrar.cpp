#ifndef CALIBRAR_CPP_
#define CALIBRAR_CPP_

#include "calibrar.h"

Calibrar :: Calibrar( QWidget *pai ) : QDialog( pai )
{
    ui = new Ui_Calibrar;

    ui->setupUi( this );

    inicializar();
}


Calibrar :: ~Calibrar()
{
    if ( quanser != NULL )
        delete quanser;

    delete temporizador;
    delete ui;
}


void Calibrar :: inicializar()
{
    canal_esc = 0;

    canal_leit_t1 = canal_leit_t2 = 0;

    nivel_t1 = nivel_t2 = 0.0;

    temporizador = new QTimer;

    quanser = NULL;

    connect( temporizador, SIGNAL( timeout() ), 
             this, SLOT( ler() ) );
}


void Calibrar :: ler()
{
    nivel_t1 = quanser->ler( canal_leit_t1 );
    nivel_t2 = quanser->ler( canal_leit_t2 );

    ui->nivel_t1->setText( QString::number( nivel_t1 ) );
    ui->nivel_t2->setText( QString::number( nivel_t2 ) );
}


void Calibrar :: on_iniciar_clicked()
{
    try
    {
        if ( quanser != NULL )
        {
            quanser->configurar_ip_porta( ui->ip_real->text(),
                                          ui->porta_real->text().toInt() );
        }
        else
        {
            quanser = new Quanser( ui->ip_real->text(),
                                   ui->porta_real->text().toInt() );
        }
    }
    catch( ExcecaoConexao e )
    {
        exibir_mensagem( this, "Erro na conexão", e.msg_erro(), Aviso );
        return;
    }

    canal_esc = ui->canal_esc->currentIndex();
    canal_leit_t1 = ui->canal_leit_t1->currentIndex();
    canal_leit_t2 = ui->canal_leit_t2->currentIndex();

    ui->ip_real->setEnabled( false );
    ui->porta_real->setEnabled( false );
    ui->canal_leit_t1->setEnabled( false );
    ui->canal_leit_t2->setEnabled( false );
    ui->parar->setEnabled( true );

    temporizador->start( 1000 );
}


void Calibrar :: on_um_clicked()
{
    quanser->escrever( canal_esc, 1.0 );
}


void Calibrar :: on_dois_clicked()
{
    quanser->escrever( canal_esc, 2.0 );
}


void Calibrar :: on_parar_clicked()
{
    ui->ip_real->setEnabled( true );
    ui->porta_real->setEnabled( true );
    ui->canal_leit_t1->setEnabled( true );
    ui->canal_leit_t2->setEnabled( true );
    ui->parar->setEnabled( false );

    quanser->desconectar();
}


void Calibrar :: on_tres_clicked()
{
    quanser->escrever( canal_esc, 3.0 );
}


void Calibrar :: on_zero_clicked()
{
    quanser->escrever( canal_esc, 0.0 );
}

#endif
