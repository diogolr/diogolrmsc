#ifndef CONFIG_MODULO_H_
#define CONFIG_MODULO_H_

#include <QDebug>

#include <QDialog>
#include <QFileDialog>
#include <QListWidgetItem>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QWidget>

#include "funcoes.h"
#include "modulo.h"
#include "ui_config_modulo.h"

class ConfigModulo : public QDialog
{
    Q_OBJECT

    // Metodos
    public:
        ConfigModulo( const QStringList &,
                      Modulo *mod = 0,
                      QWidget *pai = 0 );
        ConfigModulo( const QStringList &,
                      QWidget *pai = 0 );
                     
        ~ConfigModulo();

        Modulo::TipoModulo tipo();

        QString falha();
        QString nome();

        QStringList arquivos();

    private slots:
        void on_adicionar_clicked();
        void on_descer_clicked();
        void on_remover_clicked();
        void on_subir_clicked();

    // Atributos
    private:
        QStringList nomes_arquivos;

        Ui_ConfigModulo *ui;
};

#endif
