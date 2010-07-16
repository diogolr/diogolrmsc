#ifndef CONFIG_CANAIS_CPP_
#define CONFIG_CANAIS_CPP_

#include "config_canais.h"

ConfigCanais :: ConfigCanais( QWidget *pai ) : QDialog( pai )
{
    ui = new Ui_ConfigCanais;

    ui->setupUi( this );

    this->setModal( true );
}


ConfigCanais :: ~ConfigCanais()
{
    delete ui;
}


int ConfigCanais :: real_canal_escrita( const int &tanque )
{
    switch ( tanque )
    {
        case TANQUE_1:
            return ui->real_escrita_t1->currentIndex();
            break;

        case TANQUE_2:
            return ui->real_escrita_t2->currentIndex();
            break;

        default:
            return -1;
    }
}


int ConfigCanais :: real_canal_leitura( const int &tanque )
{
    switch ( tanque )
    {
        case TANQUE_1:
            return ui->real_leitura_t1->currentIndex();
            break;

        case TANQUE_2:
            return ui->real_leitura_t2->currentIndex();
            break;

        default:
            return -1;
    }
}


int ConfigCanais :: sim_canal_escrita( const int &tanque )
{
    switch ( tanque )
    {
        case TANQUE_1:
            return ui->sim_escrita_t1->currentIndex();
            break;

        case TANQUE_2:
            return ui->sim_escrita_t2->currentIndex();
            break;

        default:
            return -1;
    }
}


int ConfigCanais :: sim_canal_leitura( const int &tanque )
{
    switch ( tanque )
    {
        case TANQUE_1:
            return ui->sim_leitura_t1->currentIndex();
            break;

        case TANQUE_2:
            return ui->sim_leitura_t2->currentIndex();
            break;

        default:
            return -1;
    }
}


#endif
