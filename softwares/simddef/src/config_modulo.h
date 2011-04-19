#ifndef CONFIG_MODULO_H_
#define CONFIG_MODULO_H_

#include <QDebug>

#include <QDialog>
#include <QString>
#include <QStringList>

#include "ui_config_modulo.h"

class ConfigModulo : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigModulo();
        ~ConfigModulo();

        QStringList arquivos();

    private slots:
        void on_botao_ok_clicked();
        void on_botao_cancel_clicked();

    // Atributos
    private:
        QStringList nomes_arquivos;

        Ui_ConfigModulo *ui;
};

#endif
