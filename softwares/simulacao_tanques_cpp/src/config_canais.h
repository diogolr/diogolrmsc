#ifndef CONFIG_CANAIS_H_
#define CONFIG_CANAIS_H_

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QWidget>

#include "definicoes.h"
#include "ui_config_canais.h"

class ConfigCanais : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigCanais( QWidget *pai = NULL );
        ~ConfigCanais();

        int real_canal_escrita( const int & );
        int real_canal_leitura( const int & );
        int sim_canal_escrita( const int & );
        int sim_canal_leitura( const int & );

    // Atributos
    private:
        Ui_ConfigCanais *ui;
};

#endif
