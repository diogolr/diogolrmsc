#ifndef CONFIG_PARAMETROS_H_
#define CONFIG_PARAMETROS_H_

#include <QDialog>
#include <QFileDialog>
#include <QString>
#include <QWidget>

#include "funcoes.h"
#include "ui_config_parametros.h"

class ConfigParametros : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigParametros( QWidget *pai = NULL );
        ~ConfigParametros();

        QString arquivo();

    private slots:
        void on_botao_arquivo_clicked();

    // Atributos
    private:
        QString arq;

        Ui_ConfigParametros *ui;
};

#endif
